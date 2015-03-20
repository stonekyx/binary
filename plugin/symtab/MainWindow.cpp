#include <elf.h>
#include <QtGui/QMessageBox>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"
#include "DemangleWrap.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(symtab)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginSymTabMainWindow", "Symbol table"), plugin, param, parent),
    _infoModel(NULL)
{
    if(param.find("scnIndex") != param.end()) {
        _scnIndex = QString(param["scnIndex"].c_str()).toULong();
    } else {
        _scnIndex = 0;
    }
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

static size_t symTabIndex(File *file, size_t knownIdx, Elf64_Shdr *dst)
{
    if(knownIdx != 0) {
        if(!file->getShdr(knownIdx, dst) ||
                (dst->sh_type != SHT_SYMTAB &&
                 dst->sh_type != SHT_DYNSYM))
        {
            return 0;
        }
        return knownIdx;
    }
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return 0;
    }
    for(size_t i=0; i<shdrNum; i++) {
        if(!file->getShdr(i, dst)) {
            continue;
        }
        if(dst->sh_type == SHT_SYMTAB) {
            return i;
        }
    }
    return 0;
}

#define C(constant, text) \
    case (constant):return (text)

template<typename T, typename U>
bool rangeCheck(T var, U lo, U hi) {
    return var>=(T)lo && var<=(T)hi;
}

#define R(var, lo, hi, value) \
    do { if(rangeCheck(var, lo, hi)) {return (value);} }while(0)

static const char *infoBindText(unsigned char bind)
{
    switch(bind) {
        C(STB_LOCAL, "Local symbol");
        C(STB_GLOBAL, "Global symbol");
        C(STB_WEAK, "Weak symbol");
        C(STB_NUM, "Number of defined types.");
        C(STB_GNU_UNIQUE, "Unique symbol.");
    }
    R(bind, STB_LOOS, STB_HIOS, "OS-specific");
    R(bind, STB_LOPROC, STB_HIPROC, "Processor-specific");
    return "Unknown";
}

static const char *infoTypeText(unsigned char type)
{
    switch(type) {
        C(STT_NOTYPE, "Symbol type is unspecified");
        C(STT_OBJECT, "Symbol is a data object");
        C(STT_FUNC, "Symbol is a code object");
        C(STT_SECTION, "Symbol associated with a section");
        C(STT_FILE, "Symbol's name is file name");
        C(STT_COMMON, "Symbol is a common data object");
        C(STT_TLS, "Symbol is thread-local data object");
        C(STT_NUM, "Number of defined types.");
        C(STT_GNU_IFUNC, "Symbol is indirect code object");
    }
    R(type, STT_LOOS, STT_HIOS, "OS-specific");
    R(type, STT_LOPROC, STT_HIPROC, "Processor-specific");
    return "Unknown";
}

static const char *visibilityText(unsigned char other)
{
    switch(other) {
        C(STV_DEFAULT, "Default symbol visibility rules");
        C(STV_INTERNAL, "Processor specific hidden class");
        C(STV_HIDDEN, "Sym unavailable in other modules");
        C(STV_PROTECTED, "Not preemptible, not exported");
    }
    return "Unknown";
}

#undef R
#undef C

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }
    Elf64_Shdr shdr;
    _scnIndex = symTabIndex(file, _scnIndex, &shdr);
    if(_scnIndex == 0) {
        QMessageBox::critical(this, tr("Error"),
                tr("No symbol table found or not a valid symbol table."));
        close();
        return;
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(QString(), 2);

    size_t symCount = shdr.sh_size / shdr.sh_entsize;
    for(size_t i=0; i<symCount; i++) {
        Elf64_Sym sym;
        if(!file->getSym(_scnIndex, i, &sym)) {
            continue;
        }

        _infoModel->buildMore(QString("Entry %1\t%2")
                .arg(i)
                .arg(file->getStrPtr(shdr.sh_link, sym.st_name)));
        char *demangle = cplus_demangle(file->getStrPtr(shdr.sh_link, sym.st_name));
        _infoModel->buildMore(QString("\tDemangle\t%1")
                .arg(demangle));
        free(demangle);
        _infoModel->buildMore(QString("\tName offset\t0x%1")
                .arg(sym.st_name));
        _infoModel->buildMore(QString("\tInfo\t0x%1")
                .arg(sym.st_info, 0, 16));
        _infoModel->buildMore(QString("\t\tBind\t%1")
                .arg(infoBindText(ELF64_ST_BIND(sym.st_info))));
        _infoModel->buildMore(QString("\t\tType\t%1")
                .arg(infoTypeText(ELF64_ST_TYPE(sym.st_info))));
        _infoModel->buildMore(QString("\tVisibility\t%1")
                .arg(visibilityText(ELF64_ST_VISIBILITY(sym.st_other))));
        _infoModel->buildMore(QString("\tSection index\t%1")
                .arg(sym.st_shndx));
        _infoModel->buildMore(QString("\tValue\t%1")
                .arg(sym.st_value, 0, 16));
        _infoModel->buildMore(QString("\tSize\t%1")
                .arg(sym.st_size, 0, 16));
    }
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
