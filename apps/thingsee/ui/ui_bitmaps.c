/****************************************************************************
 * Copyright (C) 2014-2015 Haltian Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name NuttX nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Author: Petri Salonen <petri.salonen@haltian.com>
 *
 ****************************************************************************/

#include <nuttx/config.h>
#include <stdio.h>
#include <stdbool.h>
#include <nuttx/nx/nx.h>
#include <nuttx/nx/nxglib.h>
#include "../../../apps/system/display/oled_image.h"

#include <ui_bitmaps.h>

#define BATTERY_0_LIMIT   0
#define BATTERY_25_LIMIT  20
#define BATTERY_50_LIMIT  40
#define BATTERY_75_LIMIT  60
#define BATTERY_100_LIMIT 80

static const uint8_t g_battery0_bits[] = {
        0x3F, 0xFF, 0xFE, 0x1F, 0x7F, 0xFF, 0xFF, 0x1F, 0xE0, 0x00, 0x03, 0x9F,
        0xC0, 0x00, 0x01, 0x9F, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x01, 0xFF,
        0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x01, 0xFF, 0xC0, 0x00, 0x01, 0xFF,
        0xC0, 0x00, 0x01, 0x9F, 0xE0, 0x00, 0x03, 0x9F, 0x7F, 0xFF, 0xFF, 0x1F,
        0x3F, 0xFF, 0xFE, 0x1F
};
static const oled_image_canvas_t g_battery0_bitmap = {27, 13, g_battery0_bits};

static const uint8_t g_battery25_bits[] = {
        0x3F, 0xFF, 0xFE, 0x1F, 0x7F, 0xFF, 0xFF, 0x1F, 0xE0, 0x00, 0x03, 0x9F,
        0xCE, 0x00, 0x01, 0x9F, 0xDE, 0x00, 0x01, 0xFF, 0xDE, 0x00, 0x01, 0xFF,
        0xDE, 0x00, 0x01, 0xFF, 0xDE, 0x00, 0x01, 0xFF, 0xDE, 0x00, 0x01, 0xFF,
        0xCE, 0x00, 0x01, 0x9F, 0xE0, 0x00, 0x03, 0x9F, 0x7F, 0xFF, 0xFF, 0x1F,
        0x3F, 0xFF, 0xFE, 0x1F
};
static const oled_image_canvas_t g_battery25_bitmap = {27, 13, g_battery25_bits};

static const uint8_t g_battery50_bits[] = {
        0x3F, 0xFF, 0xFE, 0x1F, 0x7F, 0xFF, 0xFF, 0x1F, 0xE0, 0x00, 0x03, 0x9F,
        0xCE, 0xF0, 0x01, 0x9F, 0xDE, 0xF0, 0x01, 0xFF, 0xDE, 0xF0, 0x01, 0xFF,
        0xDE, 0xF0, 0x01, 0xFF, 0xDE, 0xF0, 0x01, 0xFF, 0xDE, 0xF0, 0x01, 0xFF,
        0xCE, 0xF0, 0x01, 0x9F, 0xE0, 0x00, 0x03, 0x9F, 0x7F, 0xFF, 0xFF, 0x1F,
        0x3F, 0xFF, 0xFE, 0x1F
};
static const oled_image_canvas_t g_battery50_bitmap = {27, 13, g_battery50_bits};


static const uint8_t g_battery75_bits[] = {
        0x3F, 0xFF, 0xFE, 0x1F, 0x7F, 0xFF, 0xFF, 0x1F, 0xE0, 0x00, 0x03, 0x9F,
        0xCE, 0xF7, 0x81, 0x9F, 0xDE, 0xF7, 0x81, 0xFF, 0xDE, 0xF7, 0x81, 0xFF,
        0xDE, 0xF7, 0x81, 0xFF, 0xDE, 0xF7, 0x81, 0xFF, 0xDE, 0xF7, 0x81, 0xFF,
        0xCE, 0xF7, 0x81, 0x9F, 0xE0, 0x00, 0x03, 0x9F, 0x7F, 0xFF, 0xFF, 0x1F,
        0x3F, 0xFF, 0xFE, 0x1F
};
static const oled_image_canvas_t g_battery75_bitmap = {27, 13, g_battery75_bits};

static const uint8_t g_battery100_bits[] = {
        0x3F, 0xFF, 0xFE, 0x1F, 0x7F, 0xFF, 0xFF, 0x1F, 0xE0, 0x00, 0x03, 0x9F,
        0xCE, 0xF7, 0xB9, 0x9F, 0xDE, 0xF7, 0xBD, 0xFF, 0xDE, 0xF7, 0xBD, 0xFF,
        0xDE, 0xF7, 0xBD, 0xFF, 0xDE, 0xF7, 0xBD, 0xFF, 0xDE, 0xF7, 0xBD, 0xFF,
        0xCE, 0xF7, 0xB9, 0x9F, 0xE0, 0x00, 0x03, 0x9F, 0x7F, 0xFF, 0xFF, 0x1F,
        0x3F, 0xFF, 0xFE, 0x1F
};
static const oled_image_canvas_t g_battery100_bitmap = {27, 13, g_battery100_bits};

static const uint8_t g_charging_bits[] = {
        0x03, 0xFF, 0x07, 0x7F, 0x0E, 0x7F, 0x1C, 0x7F, 0x38, 0x7F, 0x7F, 0xFF,
        0xFF, 0x7F, 0x06, 0x7F, 0x0C, 0x7F, 0x18, 0x7F, 0x30, 0x7F, 0x20, 0x7F,
        0x40, 0x7F
};
static const oled_image_canvas_t g_charging_bitmap = {9, 13, g_charging_bits};

static const uint8_t g_box_full_bits[] = {
        0x3F, 0xE7, 0x7F, 0xF7, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
        0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xF7,
        0x3F, 0xE7
};
static const oled_image_canvas_t g_box_full_bitmap = {13, 13, g_box_full_bits};

static const uint8_t g_box_empty_bits[] = {
        0x3F, 0xE7, 0x60, 0x37, 0xC0, 0x1F, 0x80, 0x0F, 0x80, 0x0F, 0x80, 0x0F,
        0x80, 0x0F, 0x80, 0x0F, 0x80, 0x0F, 0x80, 0x0F, 0xC0, 0x1F, 0x60, 0x37,
        0x3F, 0xE7
};
static const oled_image_canvas_t g_box_empty_bitmap = {13, 13, g_box_empty_bits};

static const uint8_t g_wlan_bits[] = {
        0x07, 0xE0, 0x1F, 0xF8, 0x3F, 0xFC, 0x70, 0x0E, 0xE0, 0x07, 0x47, 0xE2,
        0x0F, 0xF0, 0x1C, 0x38, 0x08, 0x10, 0x01, 0x80, 0x03, 0xC0, 0x03, 0xC0,
        0x01, 0x80
};
static const oled_image_canvas_t g_wlan_bitmap = {16, 13, g_wlan_bits};

static const uint8_t g_cellular_bits[] = {
        0x20, 0x4F, 0x40, 0x2F, 0x90, 0x9F, 0xA6, 0x5F, 0xA6, 0x5F, 0x90, 0x9F,
        0x46, 0x2F, 0x26, 0x4F, 0x06, 0x0F, 0x0F, 0x0F, 0x19, 0x8F, 0x30, 0xCF,
        0x60, 0x6F
};
static const oled_image_canvas_t g_cellular_bitmap = {12, 13, g_cellular_bits};

static const uint8_t g_bluetooth_bits[] = {
        0x31, 0x39, 0x2D, 0x27, 0xAD, 0x79, 0x31, 0x79, 0xAD, 0x27, 0x2D, 0x39,
        0x31
};
static const oled_image_canvas_t g_bluetooth_bitmap = {7, 13, g_bluetooth_bits};

static const uint8_t g_gps_bits[] = {
        0x3E, 0x7F, 0x7F, 0x7F, 0xE3, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF, 0xC1, 0xFF,
        0xE3, 0xFF, 0x7F, 0x7F, 0x7F, 0x7F, 0x3E, 0x7F, 0x3E, 0x7F, 0x1C, 0x7F,
        0x08, 0x7F
};
static const oled_image_canvas_t g_gps_bitmap = {9, 13, g_gps_bits};

static const uint8_t g_text_check_bits[] = {
        0x00, 0x11, 0x0A, 0x0C, 0xFF, 0x00, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x0D,
        0x15, 0x15, 0x15, 0x0E, 0x00, 0x0F, 0x10, 0x10, 0x08, 0xFF, 0x00, 0x00,
        0x22, 0x41, 0x41, 0x41, 0x22, 0x1C
};
static const oled_image_canvas_t g_text_check_bitmap = {8, 30, g_text_check_bits};

static const uint8_t g_text_connect_bits[] = {
        0x11, 0x11, 0xFF, 0x10, 0x01, 0x11, 0x13, 0x0E, 0x00, 0x0D, 0x15, 0x15,
        0x15, 0x0E, 0x00, 0x0F, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x0F, 0x10, 0x10,
        0x10, 0x1F, 0x00, 0x0E, 0x11, 0x11, 0x13, 0x0E, 0x00, 0x00, 0x22, 0x41,
        0x41, 0x41, 0x22, 0x1E
};
static const oled_image_canvas_t g_text_connect_bitmap = {8, 40, g_text_connect_bits};

static const uint8_t g_text_cancel_bits[] = {
        0xFF, 0x00, 0x0D, 0x15, 0x15, 0x15, 0x0E, 0x00, 0x11, 0x11, 0x11, 0x0E,
        0x00, 0x0F, 0x10, 0x10, 0x10, 0x1F, 0x00, 0x0F, 0x15, 0x15, 0x15, 0x03,
        0x00, 0x00, 0x22, 0x41, 0x41, 0x41, 0x22, 0x1E
};
static const oled_image_canvas_t g_text_cancel_bitmap = {8, 32, g_text_cancel_bits};

static const uint8_t g_text_close_bits[] = {
        0x0D, 0x15, 0x15, 0x15, 0x0E, 0x00, 0x13, 0x15, 0x15, 0x19, 0x00, 0x0E,
        0x11, 0x11, 0x13, 0x0E, 0x00, 0xFF, 0x00, 0x00, 0x22, 0x41, 0x41, 0x41,
        0x22, 0x1E
};
static const oled_image_canvas_t g_text_close_bitmap = {8, 26, g_text_close_bits};

static const uint8_t g_text_select_bits[] = {
        0x11, 0x11, 0x7F, 0x10, 0x11, 0x11, 0x1B, 0x0E, 0x00, 0x0D, 0x15, 0x15,
        0x15, 0x0E, 0x00, 0xFF, 0x00, 0x0D, 0x15, 0x15, 0x15, 0x0E, 0x00, 0x00,
        0x26, 0x4B, 0x49, 0x59, 0x32
};
static const oled_image_canvas_t g_text_select_bitmap = {8, 29, g_text_select_bits};

static const uint8_t g_text_back_bits[] = {
        0x00, 0x11, 0x0A, 0x04, 0xFF, 0x00, 0x11, 0x11, 0x11, 0x0E, 0x00, 0x0F,
        0x15, 0x15, 0x15, 0x02, 0x00, 0x00, 0x36, 0x49, 0x49, 0x49, 0x7F
};
static const oled_image_canvas_t g_text_back_bitmap = {8, 23, g_text_back_bits};

static const uint8_t g_init_screen_bits[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7F, 0xFF, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x70, 0xFC, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0xF8, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF8, 0x78, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF8, 0xF8, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xF8, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xF8, 0x00, 0x78, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xF8, 0x00, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xF0, 0xF0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xE0, 0xF0, 0x30, 0x00, 0x70, 0x38, 0x03, 0xC6, 0x07, 0xE0, 0x1F, 0x00, 0x78, 0x00, 0x00, 0x03, 0xE0, 0xF0, 0xF8, 0xF8, 0xF0, 0xFC, 0x0F, 0xFF, 0x0F, 0xE0, 0x7F, 0x83, 0xFC, 0x00, 0x00, 0x03, 0xE0, 0xF1, 0xF8, 0xF8, 0xF1, 0xFC, 0x1F, 0x7F, 0x1C, 0x01, 0xE7, 0x87, 0x3E, 0x00, 0x00, 0x03, 0xE1, 0xE3, 0xF8, 0xF8, 0xF3, 0xFC, 0x3C, 0x1E, 0x3C, 0x03, 0xC7, 0xCE, 0x1E, 0x00, 0x00, 0x07, 0xE1, 0xE6, 0xF8, 0xF8, 0xF7, 0x7C, 0x78, 0x1E, 0x3C, 0x03, 0x87, 0x9E, 0x1E, 0x00, 0x00, 0x07, 0xC1, 0xEC, 0x78, 0xF0, 0xE6, 0x78, 0xF8, 0x3E, 0x3E, 0x07, 0x87, 0xBC, 0x3C, 0x00, 0x00, 0x07, 0xC1, 0xF8, 0xF8, 0xF1, 0xFC, 0x78, 0xF0, 0x3E, 0x3F, 0x0F, 0x0F, 0x3C, 0x78, 0x00, 0x00, 0x07, 0xC1, 0xF8, 0xF0, 0xF1, 0xF8, 0x79, 0xF0, 0x7C, 0x3F, 0x8F, 0x1E, 0x78, 0xF0, 0x00, 0x00, 0x0F, 0xC3, 0xF0, 0xF1, 0xE1, 0xF8, 0x79, 0xE0, 0x7C, 0x3F, 0x9F, 0x78, 0x79, 0xE0, 0x00, 0x00, 0x0F, 0x83, 0xF0, 0xF1, 0xE1, 0xF0, 0xF9, 0xE0, 0xFC, 0x1F, 0x9F, 0xE0, 0x7F, 0x80, 0x00, 0x00, 0x0F, 0x83, 0xE0, 0xF1, 0xE3, 0xF0, 0xF1, 0xE1, 0xFC, 0x0F, 0xDF, 0x80, 0xFE, 0x00, 0x00, 0x00, 0x0F, 0x83, 0xE1, 0xF1, 0xE3, 0xE0, 0xF1, 0xE1, 0xF8, 0x07, 0xDF, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x1F, 0x87, 0xC1, 0xF3, 0xE3, 0xE0, 0xF3, 0xE7, 0x7A, 0x07, 0x9E, 0x00, 0xF8, 0x00, 0x00, 0x00, 0x1F, 0x87, 0xC1, 0xE7, 0xE7, 0xC1, 0xF3, 0xFE, 0x7B, 0x07, 0x9E, 0x01, 0xF8, 0x04, 0x00, 0x00, 0x1F, 0x87, 0x81, 0xFF, 0xFF, 0xC1, 0xFF, 0xFC, 0x7B, 0x07, 0x9F, 0x03, 0xFC, 0x1C, 0x00, 0x00, 0x1F, 0x8F, 0x81, 0xFD, 0xFF, 0x81, 0xFC, 0xF0, 0x73, 0xCF, 0x1F, 0xDF, 0x7F, 0xF8, 0x00, 0x00, 0x1F, 0x8F, 0x00, 0xF9, 0xFF, 0x80, 0xF8, 0x00, 0xF1, 0xFE, 0x0F, 0xFC, 0x3F, 0xF0, 0x00, 0x00, 0x1F, 0x0E, 0x00, 0x60, 0xC7, 0x00, 0x60, 0x00, 0xF0, 0x70, 0x03, 0xF0, 0x1F, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x01, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0F, 0x83, 0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xEF, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3F, 0xA1, 0x26, 0x23, 0xE7, 0xBE, 0x3C, 0x3F, 0xF1, 0x7F, 0x87, 0x18, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x26, 0x26, 0x0C, 0x20, 0x60, 0x21, 0x19, 0x08, 0x8D, 0x98, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x27, 0x2C, 0x08, 0x20, 0xC0, 0x21, 0x0D, 0x08, 0x98, 0xDC, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x25, 0x28, 0x0C, 0x20, 0x80, 0x21, 0x05, 0x08, 0x90, 0x54, 0x80, 0x00, 0x00, 0x00, 0x04, 0x3F, 0x25, 0xA8, 0x07, 0x3C, 0x80, 0x3D, 0x05, 0x08, 0x90, 0x56, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x24, 0xE8, 0xE1, 0xA0, 0x80, 0x21, 0x05, 0x08, 0x90, 0x53, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x24, 0xEC, 0x20, 0xA0, 0xC0, 0x21, 0x0D, 0x08, 0x98, 0xD3, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x24, 0x6E, 0x29, 0xA0, 0x60, 0x21, 0x19, 0x08, 0x8D, 0x91, 0x80, 0x00, 0x00, 0x00, 0x04, 0x21, 0x24, 0x63, 0xEF, 0x3E, 0x3C, 0x3F, 0xF1, 0x08, 0x87, 0x11, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
static const oled_image_canvas_t g_init_screen_bitmap = { 128, 64, g_init_screen_bits };

const oled_image_canvas_t *UI_get_battery_img(int percentage)
{
    const oled_image_canvas_t *retval=&g_battery0_bitmap;
    if (percentage>=BATTERY_25_LIMIT)
        retval=&g_battery25_bitmap;
    if (percentage>=BATTERY_50_LIMIT)
        retval=&g_battery50_bitmap;
    if (percentage>=BATTERY_75_LIMIT)
        retval=&g_battery75_bitmap;
    if (percentage>=BATTERY_100_LIMIT)
        retval=&g_battery100_bitmap;

    return retval;
}

const oled_image_canvas_t *UI_get_charging_img(void)
{
    return &g_charging_bitmap;
}

const oled_image_canvas_t *UI_get_box_img(bool full)
{
    if (full)
        return &g_box_full_bitmap;
    else
        return &g_box_empty_bitmap;
}

const oled_image_canvas_t *UI_get_wlan_img(void)
{
    return &g_wlan_bitmap;
}

const oled_image_canvas_t *UI_get_cellular_img(void)
{
    return &g_cellular_bitmap;
}

const oled_image_canvas_t *UI_get_bluetooth_img(void)
{
    return &g_bluetooth_bitmap;
}

const oled_image_canvas_t *UI_get_gps_img(void)
{
    return &g_gps_bitmap;
}

const oled_image_canvas_t *UI_get_text_img(bitmap_texts_t text)
{
    switch (text) {
    case TEXT_CHECK:
        return &g_text_check_bitmap;

    case TEXT_CONNECT:
        return &g_text_connect_bitmap;

    case TEXT_CANCEL:
        return &g_text_cancel_bitmap;

    case TEXT_CLOSE:
        return &g_text_close_bitmap;

    case TEXT_SELECT:
        return &g_text_select_bitmap;

    case TEXT_BACK:
        return &g_text_back_bitmap;
    }

    return NULL;
}

const oled_image_canvas_t *UI_get_init_screen_img(void)
{
    return &g_init_screen_bitmap;
}
