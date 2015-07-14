#include "Utf8Ini.h"
#include <stdio.h>

static void testSerialisation(Utf8Ini & settings)
{
    std::string data1 = settings.Serialize();
    printf("serialized:\n%s\n\n", data1.c_str());
    int errorLine = 0;
    settings.Deserialize(data1, errorLine);
    std::string data2 = settings.Serialize().c_str();
    printf("re-serialized (%s):\n%s\n\n", data1 == data2 ? "okay" : "failed", data2.c_str());
}

static void printValue(const Utf8Ini & settings, const std::string & section, const std::string & key)
{
    printf("[%s] \"%s\"=\"%s\"\n", section.c_str(), key.c_str(), settings.GetValue(section, key).c_str());
}

int main()
{
    Utf8Ini settings;

    settings.SetValue("Section 1", "Key 1", "Value 1");
    settings.SetValue("Section 1", "Key 1", "Value 2");
    settings.SetValue("Section 1", "Key 2", " this string starts and ends with a space ");
    settings.SetValue("Section 2", "Key 1", "this string contains a\nnewline and escaped characters \\ \\n ");
    testSerialisation(settings);
    puts("====================================");

    std::string iniData = "[Section 1]\n Key 1 = Value 1 \r\n Key 1 = \"Value 2\"\nKey 2 = \" this string starts and ends with a space \"\n\r ; comment line\n\n[Section 2] \nKey 1=  \"this string contains a\\nnewline and escaped characters \\\\ \\\\n \"\nKey 2 = I like Utf8Ini!";
    int errorLine;
    if(!settings.Deserialize(iniData, errorLine))
        printf("error deserializing (line %d)\n", errorLine);
    else
        testSerialisation(settings);
    puts("====================================");

    printValue(settings, "Section 1", "Key 1");
    printValue(settings, "Section 1", "Key 2");
    printValue(settings, "Section 2", "Key 1");
    printValue(settings, "Section 2", "Key 2");
    getchar();
    return 0;
}
