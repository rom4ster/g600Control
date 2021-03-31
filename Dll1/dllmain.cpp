// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include <iostream>
#include "../hidapi/hidapi.h"
#include <stdlib.h>
#include "dllmain.h"

using namespace std;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

bool G600Api::findInfo(hid_device_info** outt, hid_device_info* list, int* path) {
    hid_device_info* out = nullptr;
    if (path == nullptr) {
        hid_device_info temp;
        temp.next = list;
        out = &temp;
        while ((out = out->next) != nullptr) std::cout << out->usage_page << " " << out->usage << "\n";
        return false;
    }
    out = nullptr;
    if (list == nullptr) return false;
    out = list;
    while (out != nullptr && out->usage_page != path[0] && out->usage != path[1]) out = out->next;
    *outt = out;
    return out != nullptr;

}


bool G600Api::load()
{
    hid_device_info* info = NULL;
    int nums[]{ 65408,128 };
    auto infoList = hid_enumerate(LOGITECH_VENDOR_ID, G600_PRODUCT_ID);
    if (!((findInfo(&info, infoList, nums)))) exit(-1);
    std::cout << info->usage_page << " " << info->usage << "\n";
    hidptr = hid_open_path(info->path);
    if (hidptr != nullptr) HID_API_INIT = true;
    return HID_API_INIT;

}

bool G600Api::read(g600char* out)
{
    unsigned char* in = nullptr;
    if (out != nullptr) free(out);
    if (!HID_API_INIT) return false;
    in = (unsigned char * ) malloc(sizeof(unsigned char) * REPORT_LENGTH);
    in[0] = REPORT_ID;
    auto res = hid_get_feature_report(hidptr,in,REPORT_LENGTH);
    out = makeString(in);
    global_char = (g600char *)out;
    return res;

    
}

  G600Api::g600char**  G600Api::initMemRef()
{
      if (global_char != nullptr) free(global_char);
      global_char = nullptr;
      return &global_char;
}

  int G600Api::reportLength()
  {
      return REPORT_LENGTH;
  }

bool G600Api::write(g600char* data)
{
    if (!HID_API_INIT) return false;
    auto asHex = makeHex(data);
    auto res = hid_send_feature_report(hidptr,asHex,REPORT_LENGTH);
    if (asHex != nullptr) free(asHex);
    return res;
   
}




void G600Api::inject(g600char* data, inj_struct * values)
{
    char* dat;
    switch (values->code) {

    case  COLOR:
        if (values->len != 6) return;
         dat = data;
        dat += LOGITECH_COLOR_INDEX;
        for (int i = 0; i < values->len; i++) {
            
            dat[i] = (values->value)[i];
            
        }
        break;
    case EFFECT:
        if ((values->value)[0] != '0' || (values->value)[1] < '0' || (values->value)[1] > '2' || values->len != 2) return;
         dat = data;
        dat += LOGITECH_EFFECT_INDEX;
        dat[0] = (values->value)[0];
        dat[1] = (values->value)[1];
        break;
    case DURATION:
        if ((values->value)[0] != '0' || (values->value)[1] < '0' || (values->value)[1] > 'f' || ( (values->value)[1] > 'F' && (values->value)[1] < 'a') ||
             ( (values->value)[1]< 'A' && (values->value)[1] > '9' )||  values->len != 2) return;
         dat = data;
        dat += LOGITECH_DURATION_INDEX;
        dat[0] = (values->value)[0];
        dat[1] = (values->value)[1];
        break;
    case FREQUENCY:
        if ((values->value)[0] != '0' || (values->value)[1] < '0' || (values->value)[1] > '7' || values->len != 2) return;
        dat = data;
        dat += LOGITECH_FREQUENCY_INDEX;
        dat[0] = (values->value)[0];
        dat[1] = (values->value)[1];
        break;
    case  DPI: 
        if (values->len != 12) return;
        if ((values->value)[0] != 'A' && (values->value)[0] != 'a' && ((values->value)[0] < '0' || (values->value)[0] > '9')) return;
        if ((values->value)[0] == 'A' || (values->value)[0] == 'a') if ((values->value)[1] > '4') return;
        if ((values->value)[0] == '0') if ((values->value)[1] < '4') return;

        for (int i = 4; i < 12; i += 2) {
            if ((values->value)[0+i] != 'A' && (values->value)[0+i] != 'a' && ((values->value)[0+i] < '0' || (values->value)[0+i] > '9')) return;
            if ((values->value)[0+i] == 'A' || (values->value)[0+i] == 'a') if ((values->value)[1+i] > '4') return;
            if ((values->value)[0+i] == '0') if ((values->value)[1+i] < '4') return;
            
        }

        if ((values->value)[2] != '0' || (values->value)[3] < '0' || (values->value)[3] > '4') return;

        dat = data;
        dat += LOGITECH_DPI_INDEX;
        for (int i = 0; i < values->len; i++) {

            dat[i] = (values->value)[i];

        }
        break;


    
    }



}



G600Api::G600Api()
{
    auto res = hid_init();
    HID_API_INIT = false;
    global_char = nullptr;



}

unsigned char* G600Api::makeHex(char* str)
{
    
    auto len = strlen(str);
    auto ret = (unsigned char * ) malloc(sizeof(unsigned char) * len / 2);
    char buf[3];
    buf[2] = 0x0;
    int retnum = 0;
    for (int i = 0; i < len; i += 2) {
        buf[0] = str[i];
        buf[1] = str[i+1];
        unsigned char num = (unsigned char)strtol(buf, NULL, 16);
        ret[retnum] = num;
        retnum++;

    }
    return ret;

    
}



char* G600Api::makeString(unsigned char * buf)
{

    if (buf == nullptr) return nullptr;
    auto len = REPORT_LENGTH;
    auto ret = (char *) malloc(sizeof(char) *len *2 +1 );
    char tmp [10];
    int i;
    for ( i = 0; i < len; i++) {
       auto val = buf[i];
       #pragma warning(push)
       #pragma warning(disable: 4996)
       sprintf(tmp,"%02x ",val);
       #pragma warning(pop)
       ret[i*2] = tmp[0];
       ret[i*2 + 1] = tmp[1];
        
    }
    ret[i*2] = '\0';
    return ret;
}

G600Api::~G600Api()
{
    if (global_char != nullptr)free(global_char);
    hid_close(hidptr);
    hid_exit();
}
