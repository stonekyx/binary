#include <QtCore/QTextStream>
#include <QtGui/QMenu>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"
#include "backend/File.h"
#include "ui_MWTreeView.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(shdr)

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param, QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginShdrMainWindow", "Section header"), plugin, param, parent),
    _infoModel(NULL)
{
    _ui->infoTree->setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(
            _ui->infoTree,
            SIGNAL(customContextMenuRequested(const QPoint &)),
            this,
            SLOT(ctxMenuTreeView(const QPoint &)));
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

static const char *typeText(Elf64_Word sh_type)
{
    switch(sh_type) {
        C(SHT_NULL, "Section header table entry unused");
        C(SHT_PROGBITS, "Program data");
        C(SHT_SYMTAB, "Symbol table");
        C(SHT_STRTAB, "String table");
        C(SHT_RELA, "Relocation entries with addends");
        C(SHT_HASH, "Symbol hash table");
        C(SHT_DYNAMIC, "Dynamic linking information");
        C(SHT_NOTE, "Notes");
        C(SHT_NOBITS, "Program space with no data (bss)");
        C(SHT_REL, "Relocation entries, no addends");
        C(SHT_SHLIB, "Reserved");
        C(SHT_DYNSYM, "Dynamic linker symbol table");
        C(SHT_INIT_ARRAY, "Array of constructors");
        C(SHT_FINI_ARRAY, "Array of destructors");
        C(SHT_PREINIT_ARRAY, "Array of pre-constructors");
        C(SHT_GROUP, "Section group");
        C(SHT_SYMTAB_SHNDX, "Extended section indeces");
        C(SHT_NUM, "Number of defined types.");
        C(SHT_GNU_ATTRIBUTES, "Object attributes.");
        C(SHT_GNU_HASH, "GNU-style hash table.");
        C(SHT_GNU_LIBLIST, "Prelink library list");
        C(SHT_CHECKSUM, "Checksum for DSO content.");
        C(SHT_SUNW_move, "SUNW_move");
        C(SHT_SUNW_COMDAT, "SUNW_COMDAT");
        C(SHT_SUNW_syminfo, "SUNW_syminfo");
        C(SHT_GNU_verdef, "Version definition section.");
        C(SHT_GNU_verneed, "Version needs section.");
        C(SHT_GNU_versym, "Version symbol table.");
    }
    R(sh_type, SHT_LOSUNW, SHT_HISUNW, "Sun-specific");
    R(sh_type, SHT_LOOS, SHT_HIOS, "OS-specific");
    R(sh_type, SHT_LOPROC, SHT_HIPROC, "Processor-specific");
    R(sh_type, SHT_LOUSER, SHT_HIUSER, "Application-specific");
    return "Unknown";
}

#define APPEND(ctext, text) \
    do { \
        textStream << "\n"; \
        for(size_t i=0; i<indentLevel; i++) textStream << "\t"; \
        textStream << (ctext) << "\t" << (text); \
    } while(0)

#define IF(var, constant, ctext, text) \
    do { \
        if(!((var) & (constant))) break; \
        APPEND((ctext), (text)); \
    } while(0)

static void flagsText(QTextStream &textStream, size_t indentLevel,
        Elf64_Xword sh_flags)
{
    IF(sh_flags, SHF_WRITE, "SHF_WRITE", "Writable");
    IF(sh_flags, SHF_ALLOC, "SHF_ALLOC", "Occupies memory during execution");
    IF(sh_flags, SHF_EXECINSTR, "SHF_EXECINSTR", "Executable");
    IF(sh_flags, SHF_MERGE, "SHF_MERGE", "Might be merged");
    IF(sh_flags, SHF_STRINGS, "SHF_STRINGS", "Contains nul-terminated strings");
    IF(sh_flags, SHF_INFO_LINK, "SHF_INFO_LINK", "`sh_info' contains SHT index");
    IF(sh_flags, SHF_LINK_ORDER, "SHF_LINK_ORDER", "Preserve order after combining");
    IF(sh_flags, SHF_OS_NONCONFORMING, "SHF_OS_NONCONFORMING", "Non-standard OS specific handling required");
    IF(sh_flags, SHF_GROUP, "SHF_GROUP", "Section is member of a group.");
    IF(sh_flags, SHF_TLS, "SHF_TLS", "Section hold thread-local data.");
    IF(sh_flags, SHF_MASKOS, "SHF_MASKOS", "OS-specific.");
    IF(sh_flags, SHF_MASKPROC, "SHF_MASKPROC", "Processor-specific");
    IF(sh_flags, SHF_ORDERED, "SHF_ORDERED", "Special ordering requirement (Solaris).");
    IF(sh_flags, SHF_EXCLUDE, "SHF_EXCLUDE", "Section is excluded unless referenced or allocated (Solaris).");
}

#undef IF
#undef APPEND

#undef C
#undef D
#undef R

#define HEX(val) (QString("0x%1").arg(val, 0, 16))
#define DEC(val) (QString("%1").arg(val))

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }

    QString modelData;
    QTextStream textStream(&modelData);

    Elf64_Shdr shdr;
    size_t shdrNum;
    if(file->getShdrNum(&shdrNum) != 0) {
        return;
    }

    for(size_t i=0; i<shdrNum; i++) {
        if(!file->getShdr(i, &shdr)) {
            continue;
        }
        if(!modelData.isEmpty()) {
            textStream << "\n";
        }
        const char *name = file->getScnName(&shdr);
        textStream << "Entry " << i << "\t";
        if(name && strlen(name)) {
            textStream << name;
        } else {
            textStream << "(No name)";
        }

        textStream << "\n\tName index\t" << DEC(shdr.sh_name);
        textStream << "\n\tType\t" << typeText(shdr.sh_type);
        textStream << "\n\tFlags\t" << HEX(shdr.sh_flags);
        flagsText(textStream, 2, shdr.sh_flags);

        textStream << "\n\tVaddr at execution\t" << HEX(shdr.sh_addr);
        textStream << "\n\tFile offset\t" << HEX(shdr.sh_offset);
        textStream << "\n\tSize\t" << HEX(shdr.sh_size);
        textStream << "\n\tLink\t" << DEC(shdr.sh_link);
        textStream << "\n\tInfo\t" << HEX(shdr.sh_info);
        textStream << "\n\tSection alignment\t" << HEX(shdr.sh_addralign);
        textStream << "\n\t(sub)Entry size\t" << HEX(shdr.sh_entsize);
    }

    if(_infoModel) {
        delete _infoModel;
    }
    _infoModel = new InfoModel(modelData, 2, _ui->infoTree);
    _ui->infoTree->setModel(_infoModel);
}

void MainWindow::ctxMenuTreeView(const QPoint &pos)
{
    QMenu *menu = new QMenu;
    menu->setAttribute(Qt::WA_DeleteOnClose);
    QAction *actionShowData = menu->addAction(
            tr("Show section data"),
            this, SLOT(showSectionData()));
    actionShowData->setData(pos);
    actionShowData->setParent(menu);
    QAction *actionShowStrTab = menu->addAction(
            tr("Show as string table"),
            this, SLOT(showStringTable()));
    actionShowStrTab->setData(pos);
    actionShowStrTab->setParent(menu);
    menu->exec(_ui->infoTree->mapToGlobal(pos));
}

void MainWindow::showSectionData()
{
    QAction *actionShowData = dynamic_cast<QAction*>(sender());
    if(!actionShowData) {
        return;
    }
    QModelIndex index = _ui->infoTree->indexAt(actionShowData->data().toPoint());
    map<string, string> param;
    param["scnIndex"] = QString::number(index.row()).toUtf8().constData();
    _plugin->manager->getPlugin("ScnData")->createView(param);
}

void MainWindow::showStringTable()
{
    QAction *action = dynamic_cast<QAction*>(sender());
    if(!action) {
        return;
    }
    QModelIndex index = _ui->infoTree->indexAt(action->data().toPoint());
    map<string, string> param;
    param["scnIndex"] = QString::number(index.row()).toUtf8().constData();
    _plugin->manager->getPlugin("StrTab")->createView(param);
}

#undef HEX

END_PLUG_NAMESPACE
