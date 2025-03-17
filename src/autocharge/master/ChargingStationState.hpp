#pragma once

enum class ChargingStationState {
  OPEN,
  LOWERING_GEAR,
  CLOSED,
  ATTACHING_GEAR,
  SLAVE_CHARGE,
  LIFTING_GEAR,
};
