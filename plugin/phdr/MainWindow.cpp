#include <elf.h>
#include <QtCore/QTextStream>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(phdr)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginPhdrMainWindow", "Program header"), plugin, param, parent),
    _infoModel(NULL)
{
    updateInfo(_plugin->manager->getBackend()->getFile());
}

MainWindow::~MainWindow()
{
    if(_infoModel) {
        delete _infoModel;
    }
}

#define C(constant, value) \
    case (constant): return (value)
#define D(value) \
    default: return (value)

template<typename T, typename U>
bool rangeCheck(T var, U lo, U hi) {
    return var>=(T)lo && var<=(T)hi;
}

#define R(var, lo, hi, value) \
    do { if(rangeCheck(var, lo, hi)) {return (value);} }while(0)

static const char *typeText(Elf64_Word p_type)
{
    switch(p_type) {
        C(PT_NULL, "Program header table entry unused");
        C(PT_LOAD, "Loadable program segment");
        C(PT_DYNAMIC, "Dynamic linking information");
        C(PT_INTERP, "Program interpreter");
        C(PT_NOTE, "Auxiliary information");
        C(PT_SHLIB, "Reserved");
        C(PT_PHDR, "Entry for header table itself");
        C(PT_TLS, "Thread-local storage segment");
        C(PT_NUM, "Number of defined types");
        C(PT_GNU_EH_FRAME, "GCC .eh_frame_hdr segment");
        C(PT_GNU_STACK, "Indicates stack executability");
        C(PT_GNU_RELRO, "Read-only after relocation");
        C(PT_SUNWBSS, "Sun Specific segment");
        C(PT_SUNWSTACK, "Stack segment");
    }
    R(p_type, PT_LOSUNW, PT_HISUNW, "SUNW");
    R(p_type, PT_LOPROC, PT_HIPROC, "Processor-specific");
    R(p_type, PT_LOOS, PT_HIOS, "OS-specific");
    return "Unknown";
}

static const char *flagsText(Elf64_Word p_flags)
{
    static char buf[] = "rwx, OS-specific, Processor-specific";
    memset(buf, 0, sizeof(buf));
    memset(buf, '-', 3);
    if((p_flags & PF_R)) {
        buf[0] = 'r';
    }
    if((p_flags & PF_W)) {
        buf[1] = 'w';
    }
    if((p_flags & PF_X)) {
        buf[2] = 'x';
    }
    if((p_flags & PF_MASKOS)) {
        strncat(buf, ", OS-specific", sizeof(buf)-strlen(buf)-1);
    }
    if((p_flags & PF_MASKPROC)) {
        strncat(buf, ", Processor-specific", sizeof(buf)-strlen(buf)-1);
    }
    return buf;
}

#undef C
#undef D
#undef R

#define HEX(val) (QString("0x%1").arg(val, 0, 16))

void MainWindow::updateInfo(File *file)
{
    MWTreeView::updateInfo(file);
    if(!file) {
        return;
    }

    QString modelData;
    QTextStream textStream(&modelData);

    Elf64_Phdr phdr;
    size_t phdrNum;
    if(file->getPhdrNum(&phdrNum) != 0) {
        return;
    }
    for(size_t i=0; i<phdrNum; i++) {
        if(!file->getPhdr(i, &phdr)) {
            continue;
        }
        if(!modelData.isEmpty()) {
            textStream << "\n";
        }
        textStream << "Entry " << i+1 << "\t" << typeText(phdr.p_type);

        textStream << "\n\tFlags\t" << HEX(phdr.p_flags);
        textStream << " (" << flagsText(phdr.p_flags) << ")";

        textStream << "\n\tOffset\t" << HEX(phdr.p_offset);
        textStream << "\n\tVirtual address\t" << HEX(phdr.p_vaddr);
        textStream << "\n\tPhysical address\t" << HEX(phdr.p_paddr);
        textStream << "\n\tSize in file\t" << HEX(phdr.p_filesz);
        textStream << "\n\tSize in memory\t" << HEX(phdr.p_memsz);
        textStream << "\n\tAlignment\t" << HEX(phdr.p_align);
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(modelData);
    _ui->infoTree->setModel(_infoModel);
}

END_PLUG_NAMESPACE
