/** 
 * @file     xml_demo.h
 * @brief    
 * @details  
 * @author   华仔
 * @date     10/28/21 
 */

#ifndef SAIJIE_DEMO_XML_DEMO_H
#define SAIJIE_DEMO_XML_DEMO_H

#include "tinyxml.h"

void xml_test()
{
    std::string file_path = "/home/saite/ljh/s330_test/description.xml";

    TiXmlDocument doc(file_path);
    if(!doc.LoadFile()){
        printf("xml解析失败\n");
    }
    printf("xml解析成功\n");
    TiXmlElement* root = doc.RootElement();
    if(root){
        TiXmlElement* pobjRoads = root->FirstChildElement("Messages")->FirstChildElement("Farewell");
        std::string welcome= pobjRoads->GetText();
        printf("welcome:%s\n",welcome.c_str());


        pobjRoads = root->FirstChildElement("Class")->FirstChildElement("Students");
        TiXmlElement* student = pobjRoads->FirstChildElement("student");
        while(student){
            cout<<"phone:"<<student->FirstChildElement("phone")->GetText()<<endl;
            student = student->NextSiblingElement();
        }
    }

}


#endif //SAIJIE_DEMO_XML_DEMO_H
