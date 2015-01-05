/*
   Copyright (c) 2014, The Linux Foundation. All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.
    * Neither the name of The Linux Foundation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
   WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
   MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
   ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
   BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
   CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
   SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
   BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
   OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
   IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "vendor_init.h"
#include "property_service.h"
#include "log.h"
#include "util.h"

#include "init_msm.h"

#define CHUNK 2048 /* read 2048 bytes at a time */

int check_cmdline(char param[]) {

    char buf[CHUNK];
    FILE *file;
    size_t nread;
    file = fopen("/proc/cmdline", "r");
    if (file) {
        while ((nread = fread(buf, 1, sizeof buf, file)) > 0) {
                /* fwrite(buf, 1, nread, stdout); */
                char delims[] = " ";
                char *word = NULL;
                word = strtok(buf, delims);

                while(word != NULL) {
                        if (!strcmp(param,word)) {
                                fclose(file);
                                return 1;
                        }
                        word = strtok(NULL, delims);
                }
        }
    }	
    fclose(file);
    return 0;
}

 
void init_msm_properties(unsigned long msm_id, unsigned long msm_ver, char *board_type)
{
    char serial[PROP_VALUE_MAX];
    char device[PROP_VALUE_MAX];
    char devicename[PROP_VALUE_MAX];

    UNUSED(msm_id);
    UNUSED(msm_ver);
    UNUSED(board_type);

    property_get("ro.boot.serialno", serial);
    if (strncmp(serial, "LGD320", 6) == 0) {
        if (check_cmdline("model.name=LG-D320n") == 1) {
                property_set("ro.product.device", "w5");
                property_set("ro.product.model", "LG-D320n");
                property_set("ro.nfc.port", "I2C");
        } else {
                property_set("ro.product.device", "w5");
                property_set("ro.product.model", "LG-D320");
        }
        property_set("ro.build.description", "w5_global_com-user 4.4.2 KOT49I.A1398228431 1398228431 release-keys");
        property_set("ro.build.fingerprint", "lge/w5_global_com/w5:4.4.2/KOT49I.A1398228431/1398228431:user/release-keys");
        property_set("persist.radio.multisim.config", "");
        property_set("telephony.lteOnCdmaDevice", "0");
    } else if (strncmp(serial, "LGD325", 6) == 0) {
        property_set("ro.product.device", "w5ds");
        property_set("ro.product.model", "LG-D325");
        property_set("ro.build.description", "w5ds_global_com-user 4.4.2 KOT49I.D41510c D41510c.1393916607 release-keys");
        property_set("ro.build.fingerprint", "lge/w5ds_global_com-user/w5ds:4.4.2/KOT49I.D41510c/D41510c.1393916607:user/release-keys");
                property_set("persist.radio.multisim.config", "dsds");
        property_set("telephony.lteOnCdmaDevice", "0");
    } else if (strncmp(serial, "LGMS323", 7) == 0) {
        property_set("ro.product.device", "w5");
        property_set("ro.product.model", "LG-MS323");
        property_set("ro.build.description", "w5_global_com-user 4.4.2 KOT49I.D41510c D41510c.1393916607 release-keys");
        property_set("ro.build.fingerprint", "lge/w5_global_com-user/w5ds:4.4.2/KOT49I.D41510c/D41510c.1393916607:user/release-keys");
                property_set("persist.radio.multisim.config", "");
        property_set("telephony.lteOnCdmaDevice", "0");
    } else {
        /* XXX */
        property_set("ro.product.device", "w5");
        property_set("ro.product.model", "Please write your model name to vm03@yandex.ru");
        property_set("persist.radio.multisim.config", "");
        property_set("telephony.lteOnCdmaDevice", "0");
    }
    property_get("ro.product.device", device);
    strlcpy(devicename, device, sizeof(devicename));
    ERROR("Found hardware id: %s setting build properties for %s device\n", serial, devicename);
}
