#include "computationThread.h"
#include "gpsParser.h"
#include "xbeeParser.h"

#include "parserThread.h"

#include "UIThread.h"

#include <math.h>

extern uint8_t g_myID;
// extern float g_myLongitude;
// extern float g_myLatitude;

typedef struct {
  float longitudes[MAX_PEERS];
  float latitudes[MAX_PEERS];
  int8_t msgs[MAX_PEERS];
  float alert_distance;
} snapshot_param_t;

static snapshot_param_t params;
alert_message_t alerts[MAX_PEERS];

float distance(float lo1, float la1, float lo2, float la2) {
  float a = pow(sin((la1 - la2) / 2.f), 2.f) + cos(la1) * cos(la2) * pow(sin((lo1 - la2) / 2.f), 2.f);
  float c = 2 * atan2(sqrt(a), sqrt(1 - a));
  float d = 6378137 * c;
  return d;
}

float bearing(float lo1, float la1, float lo2, float la2) {
  float theta = atan2(sin(lo1 - lo2) * cos(la2),
                      cos(la1) * sin(la2) - sin(la1) * cos(la2) * cos(lo1 - lo2));
  return theta;
}

/* TODO: THIS IS A NAIVE ALGORITHM! Only single outlier is supported.*/
void compute(void) {
  // TODO: NO TIMESTAMP!
  int8_t i, j;
  for (i = 0; i < MAX_PEERS; ++i) {
    /* Find the closest peer.*/
    int8_t min_peer = PEERID_NONE;
    float min_dist = INFINITY;
    for (j = 0; j < MAX_PEERS; ++j) {
      if (i != j) {
        float d = distance(params.longitudes[i], params.latitudes[i],
                           params.longitudes[j], params.latitudes[j]);
        if (d < min_dist) {
          min_dist = d;
          min_peer = PEERID_NONE;
        }
      }
    }
    /* If someone is too far away or if they set emergency flag, alert.*/
    if (min_dist > params.alert_distance || params.msgs[i] == MSG_EMERGENCY) {
      /* If this device is too far away, point to the closest peer.*/
      if (i == g_myID) {
        float br = bearing(params.longitudes[i], params.latitudes[i],
                           params.longitudes[j], params.latitudes[j]);
        alerts[g_myID].bearing = br * 360 / M_PI;
        alerts[g_myID].distance = min_dist;
      }
      /* If another device is too far away, point from this device to that one.*/
      else {
        float br = bearing(params.longitudes[g_myID], params.latitudes[g_myID],
                           params.longitudes[i], params.latitudes[i]);
        float d = distance(params.longitudes[g_myID], params.latitudes[g_myID],
                           params.longitudes[i], params.latitudes[i]);
        alerts[i].bearing = br * 360 / M_PI;
        alerts[i].distance = d;
      }
    }
    /* If someone is within range.*/
    else {
      alerts[i].bearing = ALERT_NONE;
      alerts[i].distance = ALERT_NONE;
    }
  }
}

THD_WORKING_AREA(waTdComp, COMP_WA_SIZE);
THD_FUNCTION(tdComp, arg) {
  (void)arg;

  /* Initializes.*/
  {
    int8_t i;
    for (i = 0; i < MAX_PEERS; ++i) {
      alerts[i].bearing = ALERT_NONE;
      alerts[i].distance = ALERT_NONE;
    }
  }

  info_computation("Spawned\r\n");
  while (true) {
    msg_t p;

    /* First, update own position.*/
    params.longitudes[g_myID] = getGPSLongitude();
    // g_myLongitude = params.longitudes[g_myID] * 180.f / M_PI;
    params.latitudes[g_myID] = getGPSLatitude();
    UIUpdateMyPosition(params.longitudes[g_myID] * 180.f / M_PI,
                       params.latitudes[g_myID] * 180.f / M_PI);
    // g_myLatitude = params.latitudes[g_myID] * 180.f / M_PI;

    /* If a new message is received, compute immediately.*/
    if (chMBFetch(&xbeeMailbox, &p, TIME_IMMEDIATE) == MSG_OK) {
      /* Update state.*/
      {
        /* Copy the message.*/
        peer_message_t peer = *((peer_message_t *)p);
        int8_t id = peer.peerID;
        /* Free the message.*/
        chPoolFree(&xbeeMemoryPool, (peer_message_t *)p);
        /* Actually update the info.*/
        params.longitudes[id] = peer.longitude;
        params.latitudes[id] = peer.latitude;
        params.msgs[id] = peer.msgID;

        /* Debug information.*/
        {
#include "LCD.h"
          float degree = truncf(peer.longitude * 180.f / M_PI);
          float minute = (peer.longitude - (degree * M_PI / 180.f)) * 10800.f / M_PI;
          info_computation("Peer ID: %d\r\n", peer.peerID);
          info_computation("Longitude Radian: %.6f\r\n", peer.longitude);
          info_computation("Longitude Deg: %.f\r\n", degree);
          info_computation("Longitude Min: %.3f\r\n", minute);
          degree = truncf(peer.latitude * 180.f / M_PI);
          minute = (peer.latitude - (degree * M_PI / 180.f)) * 10800.f / M_PI;
          info_computation("Latitude Radian: %.6f\r\n", peer.latitude);
          info_computation("Latitude Deg: %.f\r\n", degree);
          info_computation("Latitude Min: %.3f\r\n", minute);
          info_computation("Msg ID: %d\r\n", peer.msgID);
        }
      }

      compute();
    }
    /* Otherwise, compute every time period.*/
    else {
      compute();
    }
  }
}