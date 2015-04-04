#include <iostream>
#include <cstdio>
#include <elf.h>

#include "ui_MWTreeView.h"
#include "Defines.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);
USE_PLUG_NAMESPACE(plugin_framework);

BEGIN_PLUG_NAMESPACE(basic)

static QString modelData =
"Kind	\002INFO_FIELD_KIND\003\n"
"Class	\002INFO_FIELD_CLASS\003\n"
"Executable Header	\002INFO_FIELD_EHDR\003\n"
"	Type	\002INFO_FIELD_EHDR_TYPE\003\n"
"	Architecture	\002INFO_FIELD_EHDR_ARCH\003\n"
"	Version	\002INFO_FIELD_EHDR_VER\003\n"
"	Entry point	\002INFO_FIELD_EHDR_ENTRY\003\n"
"	PHT offset	\002INFO_FIELD_EHDR_PHTOFF\003\n"
"	SHT offset	\002INFO_FIELD_EHDR_SHTOFF\003\n"
"	Flags	\002INFO_FIELD_EHDR_FLAGS\003\n"
"	Header size	\002INFO_FIELD_EHDR_HEADERSIZE\003\n"
"	PHT entry size	\002INFO_FIELD_EHDR_PHTENTSIZE\003\n"
"	PHT entry count	\002INFO_FIELD_EHDR_PHTNUM\003\n"
"	SHT entry size	\002INFO_FIELD_EHDR_SHTENTSIZE\003\n"
"	SHT entry count	\002INFO_FIELD_EHDR_SHTNUM\003\n"
"	String table index	\002INFO_FIELD_EHDR_STRNDX\003";

MainWindow::MainWindow(BIN_NAMESPACE(frontend)::Plugin *plugin,
        map<string, string> param,
        QWidget *parent) :
    MWTreeView(new Ui::MWTreeView("PluginBasicMainWindow", "Basic information"),
            plugin, param, parent),
    _infoModel(new InfoModel(modelData, 2))
{
    _ui->infoTree->setModel(_infoModel);
    updateInfo();
}

MainWindow::~MainWindow()
{
    delete _infoModel;
}

#define SET(key) \
    _infoModel->setData(key, QApplication::translate("PluginBasicMainWindow", rawStr, 0, QApplication::UnicodeUTF8))

#define SETQ(key) \
    _infoModel->setData(key, rawQStr)

#define SETHEX(key, val) \
    do{ _infoModel->setData(key, QString("0x%1").arg(val, 0, 16)); }while(0)

#define SETDEC(key, val) \
    do{ _infoModel->setData(key, QString("%1").arg(val)); }while(0)

#define RESET(key) \
    do{ rawStr="\002"key"\003"; SET(key); }while(0)

#define C(constant, value) \
    case (constant): rawStr=(value); break
#define D(value) \
    default: rawStr=(value); break

template<typename T, typename U>
bool rangeCheck(T var, U lo, U hi) {
    return var>=(T)lo && var<=(T)hi;
}

#define R(var, lo, hi, value) \
    do { if(rangeCheck(var, lo, hi)) {rawStr=(value);} }while(0)

void MainWindow::updateInfo(File *file)
{
    if(!_ui->switchMode(file)) {
        return;
    }

    const char *rawStr;
    QString rawQStr;
    size_t sizeVal;

    int elfClass = file->getClass();
    rawStr = Defines::commentText(elfClass, defines_ELFCLASS);
    SET("INFO_FIELD_CLASS");

    File::ELFKind elfKind = file->getKind();
    switch(elfKind) {
        C(File::KIND_AR, "ar(1) archive");
        C(File::KIND_ELF, "ELF object");
        C(File::KIND_COFF, "COFF");
        D("Unknown");
    }
    SET("INFO_FIELD_KIND");

    Elf64_Ehdr ehdr;
    if(file->getEhdr(&ehdr)) {
        rawStr = "Yes";
        SET("INFO_FIELD_EHDR");

        rawStr = Defines::commentText(ehdr.e_type, defines_ET);
        SET("INFO_FIELD_EHDR_TYPE");

        rawStr = Defines::commentText(ehdr.e_machine, defines_EM);
        SET("INFO_FIELD_EHDR_ARCH");

        rawStr = Defines::commentText(ehdr.e_version, defines_EV);
        SET("INFO_FIELD_EHDR_VER");

        SETHEX("INFO_FIELD_EHDR_ENTRY", ehdr.e_entry);
        SETHEX("INFO_FIELD_EHDR_PHTOFF", ehdr.e_phoff);
        SETHEX("INFO_FIELD_EHDR_SHTOFF", ehdr.e_shoff);
        SETHEX("INFO_FIELD_EHDR_FLAGS", ehdr.e_flags);
        SETHEX("INFO_FIELD_EHDR_HEADERSIZE", ehdr.e_ehsize);
        SETHEX("INFO_FIELD_EHDR_PHTENTSIZE", ehdr.e_phentsize);

        if(file->getPhdrNum(&sizeVal) != 0) {
            rawQStr = QString("0x%1 (Unknown)").arg(ehdr.e_phnum, 0, 16);
        } else {
            rawQStr = QString("0x%1 (%2)").arg(ehdr.e_phnum, 0, 16).arg(sizeVal);
        }
        SETQ("INFO_FIELD_EHDR_PHTNUM");

        SETHEX("INFO_FIELD_EHDR_SHTENTSIZE", ehdr.e_shentsize);

        if(file->getShdrNum(&sizeVal) != 0) {
            rawQStr = QString("0x%1 (Unknown)").arg(ehdr.e_shnum, 0, 16);
        } else {
            rawQStr = QString("0x%1 (%2)").arg(ehdr.e_shnum, 0, 16).arg(sizeVal);
        }
        SETQ("INFO_FIELD_EHDR_SHTNUM");

        if(file->getShdrStrNdx(&sizeVal) != 0) {
            rawQStr = QString("0x%1 (Unknown)").arg(ehdr.e_shstrndx, 0, 16);
        } else {
            rawQStr = QString("0x%1 (%2)").arg(ehdr.e_shstrndx, 0, 16).arg(sizeVal);
        }
        SETQ("INFO_FIELD_EHDR_STRNDX");

    } else {
        rawStr = "No";
        SET("INFO_FIELD_EHDR");
        RESET("INFO_FIELD_EHDR_TYPE");
        RESET("INFO_FIELD_EHDR_ARCH");
        RESET("INFO_FIELD_EHDR_VER");
        RESET("INFO_FIELD_EHDR_ENTRY");
        RESET("INFO_FIELD_EHDR_PHTOFF");
        RESET("INFO_FIELD_EHDR_SHTOFF");
        RESET("INFO_FIELD_EHDR_FLAGS");
        RESET("INFO_FIELD_EHDR_HEADERSIZE");
        RESET("INFO_FIELD_EHDR_PHTENTSIZE");
        RESET("INFO_FIELD_EHDR_PHTNUM");
        RESET("INFO_FIELD_EHDR_SHTENTSIZE");
        RESET("INFO_FIELD_EHDR_SHTNUM");
        RESET("INFO_FIELD_EHDR_STRNDX");
    }
}

#undef SET

END_PLUG_NAMESPACE
