/* example.c
Copyright 2021 Carl John Kugler III

Licensed under the Apache License, Version 2.0 (the License); you may not use
this file except in compliance with the License. You may obtain a copy of the
License at

   http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software distributed
under the License is distributed on an AS IS BASIS, WITHOUT WARRANTIES OR
CONDITIONS OF ANY KIND, either express or implied. See the License for the
specific language governing permissions and limitations under the License.
*/
#include <stdio.h>
//
#include "FreeRTOS.h"
#include "task.h"
#include "FreeRTOS_time.h"
//
#include "pico/stdlib.h"
#include "pico/multicore.h" // get_core_num()

//
#include "crash.h"
#include "stdio_cli.h"
#include "hardware/rtc.h"

#include <elf.h>



void readDS3231(datetime_t *rtc);

static void prvLaunchRTOS() {
}

int main() {

    crash_handler_init();
    stdio_init_all();

    FreeRTOS_time_init();


    CLI_Start();

    // [mee] add - set RTC time from DS3231
    datetime_t t;
    readDS3231(&t);
    setrtc(&t);

    printf("Core %d: Launching FreeRTOS scheduler\n", get_core_num());
    /* Start the tasks and timer running. */
    vTaskStartScheduler();
    /* should never reach here */
    panic_unsupported();

    return 0;
}
