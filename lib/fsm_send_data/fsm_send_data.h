#ifndef FSM_SEND_DATA_H
#define FSM_SEND_DATA_H

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

#include "fsm.h"
#include "timer.h"
#include "fsm_hit_detection.h"
#include "fsm_door_checking.h"
#include "fsm_led_alarm.h"
#include "MQTTClient.h"

enum fsm_send_data_state {
  IDLE,
  SENDING,
};

typedef struct {
  fsm_t               fsm_send_data;
  uint8_t             active;
  uint32_t            sampling_rate;
  uint32_t            timeout;
  MQTTClient          client;
  fsm_hit_detection_t fsm_hit[N_ACCEL];
  uint8_t             n_accel;
  fsm_door_checking_t fsm_door[N_DOORS];
  uint8_t             n_doors;
  fsm_led_alarm_t     fsm_led[N_LEDS];
  uint8_t             n_leds;
} fsm_send_data_t;

void fsm_send_data_init(fsm_send_data_t* this, uint32_t sampling_ms, fsm_led_alarm_t fsm_led[], uint8_t n_leds, fsm_door_checking_t fsm_door[], uint8_t n_doors, fsm_hit_detection_t fsm_hit[], uint8_t n_accel);
void fsm_send_data_set_active (fsm_send_data_t* this, uint8_t val);

#endif // FSM_SEND_DATA_H