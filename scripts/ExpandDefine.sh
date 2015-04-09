#!/bin/bash

sed -e '/\/\*.*\*\//d' -e '/\/\*/,/\*\//d' |
awk 'BEGIN{FS=", "; OFS=", "; rangeCount=0; count=0;}
{
    curfield = "";
    fieldCnt = 0;
    wait = 0;
    for(i=1; i<=NF; i++) {
        if(length(curfield)>0) {
            curfield = curfield FS;
        }
        curfield = curfield $i;
        if(substr($i, 1, 1) == "\"") {
            wait = 1;
        }
        if(wait==1 && substr($i, length($i), 1) != "\"") {
            continue;
        }
        wait = 0;
        fieldCnt ++;
        fields[fieldCnt] = curfield;
        curfield = "";
    }
    NF = fieldCnt;
    for(i=1; i<=fieldCnt; i++) {
        $i = fields[i];
    }
    if($1=="BINARY_PLUGIN_VALUE_TYPE") {$1=""; valueType=substr($0, 3); next;}
    if($1=="BINARY_PLUGIN_NAMESPACE") {$1=""; namespace=substr($0, 3); next;}
    if($1=="BINARY_PLUGIN_OBJECTNAME") {$1=""; objName=substr($0, 3); next;}
    if(NF>=3){rangeX[rangeCount]=$1;rangeY[rangeCount]=$2;rangeZ[rangeCount]=$3;rangeCount++;}
    else if(NF>=2) {itemX[count]=$1;itemY[count]=$2;count++;}
    else if(NF>=1) {itemX[count]=$1;itemY[count]="\"" $1 "\"";count++;}
}
END {
    className = objName;
    className = toupper(substr(className, 1, 1)) substr(className, 2);
    className = className "Initializer";
    print "#ifndef PLUGIN_PLUGIN_FRAMEWORK_" toupper(objName) "_H";
    print "#define PLUGIN_PLUGIN_FRAMEWORK_" toupper(objName) "_H";
    print "";
    print "#include <elf.h>";
    print "#include \"ExpandDefine.h\"";
    print "";
    print "#include \"common.h\"";
    print "";
    print "BEGIN_PLUG_NAMESPACE(" namespace ")";
    print "";
    print "PLUG_NAMESPACE(plugin_framework)::ExpandDefine<" valueType " > " objName ";";
    print "";
    print "static struct " className " {";
    print "    " className "();";
    print "} unusedInstance" className ";";
    print "";
    print className "::" className "() {";
    print "using namespace PLUG_NAMESPACE(plugin_framework);";
    for(i=0; i<count; i++) {
        print "#ifdef " itemX[i];
        print "    do {";
        print "        DefineInfo<ExpandDefine<" valueType " >::InfoType> entry;";
        print "        entry.name = \"" itemX[i] "\";";
        print "        entry.explain = " itemY[i] ";";
        print "        " objName ".defineMap[" itemX[i] "] = entry;";
        print "    } while(0);";
        print "#endif";
    }
    for(i=0; i<rangeCount; i++) {
        commonX = rangeX[i];
        commonY = rangeY[i];
        sub("LO", "", commonX);
        sub("HI", "", commonY);
        if(commonX != commonY) continue;
        print "#if defined(" rangeX[i] ") && defined(" rangeY[i] ")";
        print "    do {";
        print "        DefineInfo<ExpandDefine<" valueType " >::InfoType> entry;";
        print "        entry.name = \"" commonX "\";";
        print "        entry.explain = " rangeZ[i] ";";
        print "        " objName ".rangeMap[std::make_pair(" rangeX[i] ", " rangeY[i] ")] = entry;";
        print "        " objName ".rangeSet.insert(" rangeX[i] ");";
        print "        " objName ".rangeSet.insert(" rangeY[i] ");";
        print "    } while(0);";
        print "#endif"
    }
    print "}";
    print "";
    print "END_PLUG_NAMESPACE";
    print "";
    print "#endif";
}
'
