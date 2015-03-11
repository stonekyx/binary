#include <iostream>
#include <elf.h>
#include <QKeyEvent>
#include <QtGui/QHeaderView>

#include "frontend/PluginManager.h"
#include "backend/Backend.h"

#include "ui_MainWindow.h"

#include "MainWindow.h"

using namespace std;

USE_BIN_NAMESPACE(backend);

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
    QMainWindow(parent),
    ui(new Ui::MainWindow()),
    _plugin(plugin),
    _infoModel(new InfoModel(modelData))
{
    ui->setupUi(this);
    ui->infoTree->setModel(_infoModel);
    ui->infoTree->header()->resizeSection(0, 180);
    QObject::connect(_plugin->manager->getBackend(),
            SIGNAL(fileChanged(binary::backend::File *)),
            this, SLOT(updateInfo(binary::backend::File *)));
    updateInfo(_plugin->manager->getBackend()->getFile());
    setAttribute(Qt::WA_DeleteOnClose);
    show();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete _infoModel;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    switch(event->key()) {
    case Qt::Key_W:
        if(event->modifiers() & Qt::ControlModifier)
            close();
        break;
    default:
        break;
    }
}

#define SET(key) \
    _infoModel->setData(key, QApplication::translate("PluginBasicMainWindow", rawStr, 0, QApplication::UnicodeUTF8))

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
    if(!file) {
        ui->infoTree->hide();
        ui->defaultLabel->show();
        return;
    }
    ui->defaultLabel->hide();
    ui->infoTree->show();

    const char *rawStr;

    int elfClass = file->getClass();
    switch(elfClass){
        C(ELFCLASS32, "32 bit");
        C(ELFCLASS64, "64 bit");
        D("Unknown");
    }
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

        switch(ehdr.e_type) {
            C(ET_NONE, "No file type");
            C(ET_REL, "Relocatable file");
            C(ET_EXEC, "Executable file");
            C(ET_DYN, "Shared object file");
            C(ET_CORE, "Core file");
            C(ET_NUM, "Number of defined types");
            D("Unknown");
        }
        R(ehdr.e_type, ET_LOOS, ET_HIOS, "OS-specific");
        R(ehdr.e_type, ET_LOPROC, ET_HIPROC, "Processor-specific");
        SET("INFO_FIELD_EHDR_TYPE");

        switch(ehdr.e_machine) {
            C(EM_NONE, "No machine");
            C(EM_M32, "AT&T WE 32100");
            C(EM_SPARC, "SUN SPARC");
            C(EM_386, "Intel 80386");
            C(EM_68K, "Motorola m68k family");
            C(EM_88K, "Motorola m88k family");
            C(EM_860, "Intel 80860");
            C(EM_MIPS, "MIPS R3000 big-endian");
            C(EM_S370, "IBM System/370");
            C(EM_MIPS_RS3_LE, "MIPS R3000 little-endian");

            C(EM_PARISC, "HPPA");
            C(EM_VPP500, "Fujitsu VPP500");
            C(EM_SPARC32PLUS, "Sun's \"v8plus\"");
            C(EM_960, "Intel 80960");
            C(EM_PPC, "PowerPC");
            C(EM_PPC64, "PowerPC 64-bit");
            C(EM_S390, "IBM S390");

            C(EM_V800, "NEC V800 series");
            C(EM_FR20, "Fujitsu FR20");
            C(EM_RH32, "TRW RH-32");
            C(EM_RCE, "Motorola RCE");
            C(EM_ARM, "ARM");
            C(EM_FAKE_ALPHA, "Digital Alpha");
            C(EM_SH, "Hitachi SH");
            C(EM_SPARCV9, "SPARC v9 64-bit");
            C(EM_TRICORE, "Siemens Tricore");
            C(EM_ARC, "Argonaut RISC Core");
            C(EM_H8_300, "Hitachi H8/300");
            C(EM_H8_300H, "Hitachi H8/300H");
            C(EM_H8S, "Hitachi H8S");
            C(EM_H8_500, "Hitachi H8/500");
            C(EM_IA_64, "Intel Merced");
            C(EM_MIPS_X, "Stanford MIPS-X");
            C(EM_COLDFIRE, "Motorola Coldfire");
            C(EM_68HC12, "Motorola M68HC12");
            C(EM_MMA, "Fujitsu MMA Multimedia Accelerator");
            C(EM_PCP, "Siemens PCP");
            C(EM_NCPU, "Sony nCPU embeeded RISC");
            C(EM_NDR1, "Denso NDR1 microprocessor");
            C(EM_STARCORE, "Motorola Start*Core processor");
            C(EM_ME16, "Toyota ME16 processor");
            C(EM_ST100, "STMicroelectronic ST100 processor");
            C(EM_TINYJ, "Advanced Logic Corp. Tinyj emb.fam");
            C(EM_X86_64, "AMD x86-64 architecture");
            C(EM_PDSP, "Sony DSP Processor");

            C(EM_FX66, "Siemens FX66 microcontroller");
            C(EM_ST9PLUS, "STMicroelectronics ST9+ 8/16 mc");
            C(EM_ST7, "STmicroelectronics ST7 8 bit mc");
            C(EM_68HC16, "Motorola MC68HC16 microcontroller");
            C(EM_68HC11, "Motorola MC68HC11 microcontroller");
            C(EM_68HC08, "Motorola MC68HC08 microcontroller");
            C(EM_68HC05, "Motorola MC68HC05 microcontroller");
            C(EM_SVX, "Silicon Graphics SVx");
            C(EM_ST19, "STMicroelectronics ST19 8 bit mc");
            C(EM_VAX, "Digital VAX");
            C(EM_CRIS, "Axis Communications 32-bit embedded processor");
            C(EM_JAVELIN, "Infineon Technologies 32-bit embedded processor");
            C(EM_FIREPATH, "Element 14 64-bit DSP Processor");
            C(EM_ZSP, "LSI Logic 16-bit DSP Processor");
            C(EM_MMIX, "Donald Knuth's educational 64-bit processor");
            C(EM_HUANY, "Harvard University machine-independent object files");
            C(EM_PRISM, "SiTera Prism");
            C(EM_AVR, "Atmel AVR 8-bit microcontroller");
            C(EM_FR30, "Fujitsu FR30");
            C(EM_D10V, "Mitsubishi D10V");
            C(EM_D30V, "Mitsubishi D30V");
            C(EM_V850, "NEC v850");
            C(EM_M32R, "Mitsubishi M32R");
            C(EM_MN10300, "Matsushita MN10300");
            C(EM_MN10200, "Matsushita MN10200");
            C(EM_PJ, "picoJava");
            C(EM_OPENRISC, "OpenRISC 32-bit embedded processor");
            C(EM_ARC_A5, "ARC Cores Tangent-A5");
            C(EM_XTENSA, "Tensilica Xtensa Architecture");
            C(EM_AARCH64, "ARM AARCH64");
            C(EM_TILEPRO, "Tilera TILEPro");
            C(EM_MICROBLAZE, "Xilinx MicroBlaze");
            C(EM_TILEGX, "Tilera TILE-Gx");

            C(EM_ALPHA, "Alpha");
            D("Unknown");
        }
        SET("INFO_FIELD_EHDR_ARCH");

        switch(ehdr.e_version) {
            C(EV_NONE, "Invalid ELF version");
            C(EV_CURRENT, "Current version");
            D("Unknown");
        }
        SET("INFO_FIELD_EHDR_VER");

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
