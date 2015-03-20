#!/bin/bash

sed -e '/\/\*.*\*\//d' -e '/\/\*/,/\*\//d' |
awk 'BEGIN{FS=", "; rangeCount=0; count=0;}
{
    if(NF>=3){rangeX[rangeCount]=$1;rangeY[rangeCount]=$2;rangeZ[rangeCount]=$3;rangeCount++;}
    else if(NF>=2) {
        if($1=="BINARY_PLUGIN_VALUE_TYPE") {valueType=$2; next;}
        if($1=="BINARY_PLUGIN_NAMESPACE") {namespace=$2; next;}
        if($1=="BINARY_PLUGIN_OBJECTNAME") {objName=$2; next;}
        itemX[count]=$1;itemY[count]=$2;count++;}
    else if(NF>=1) {itemX[count]=$1;itemY[count]="\"" $1 "\"";count++;}
}
END {
    className = objName;
    className = toupper(substr(className, 1, 1)) substr(className, 2);
    className = className "Initializer";
    print "#include <elf.h>";
    print "#include <ExpandDefine.h>";
    print "";
    print "#include \"common.h\"";
    print "";
    print "BEGIN_PLUG_NAMESPACE(" namespace ")";
    print "";
    print "static PLUG_NAMESPACE(plugin_framework)::ExpandDefine<" valueType "> " objName ";";
    print "";
    print "static struct " className " {";
    print "    " className "();";
    print "} unusedInstance;";
    print "";
    print className "::" className "() {";
    print "using namespace PLUG_NAMESPACE(plugin_framework);";
    for(i=0; i<count; i++) {
        print "#ifdef " itemX[i];
        print "    do {";
        print "        DefineInfo entry;";
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
        print "        DefineInfo entry;";
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
}
'
