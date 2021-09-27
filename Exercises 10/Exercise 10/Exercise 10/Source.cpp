#include "hippomocks.h"
#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include <functional>
#include <stdexcept>



struct SpeedUpdate {
	double velocity_mps;
};
struct CarDetected {
	double distance_m;
	double velocity_mps;
};
struct BrakeCommand {
	double time_to_collision_s;
};

struct SpeedLimitDetected {
	unsigned short speed_mps;
};

using SpeedUpdateCallback = std::function<void(const SpeedUpdate&)>;
using CarDetectedCallback = std::function<void(const CarDetected&)>;
using SpeedLimitCallback = std::function<void(const SpeedLimitDetected&)>;
struct IServiceBus {
	virtual ~IServiceBus() = default;
	virtual void publish(const BrakeCommand&) = 0;
	virtual void subscribe_to_speed(SpeedUpdateCallback) = 0;
	virtual void subscribe_to_car_detected(CarDetectedCallback) = 0;
	virtual void subscribe_to_speed_limit(SpeedLimitCallback) = 0;
};
struct AutoBrake {
	AutoBrake(IServiceBus& bus) : collision_threshold_s{ 5 }, speed_mps{}, speed_limit{ 39 } {
		bus.subscribe_to_speed([this, &bus](const SpeedUpdate& update) { 
			speed_mps = update.velocity_mps;
			if (speed_mps > speed_limit) {
				bus.publish(BrakeCommand{ 0 });
			}
			});
		bus.subscribe_to_car_detected([this, &bus](const CarDetected& cd) {
			auto relative_velocity_mps = speed_mps - cd.velocity_mps;
			auto time_to_collision_s = cd.distance_m / relative_velocity_mps;
			if (time_to_collision_s > 0 && time_to_collision_s <= collision_threshold_s) {
				bus.publish(BrakeCommand{ time_to_collision_s });
			}
			});
		bus.subscribe_to_speed_limit([this, &bus](const SpeedLimitDetected& limit) { 
			speed_limit = limit.speed_mps; 
			if (speed_mps > speed_limit) {
				bus.publish(BrakeCommand{ 0 });
			}
			});
	}
	void set_collision_threshold_s(double x) {
		if (x < 1)
			throw std::runtime_error{ "Collision less than 1." };
		collision_threshold_s = x;
	}
	double get_collision_threshold_s() const {
		return collision_threshold_s;
	}
	double get_speed_mps() const {
		return speed_mps;
	}

	double get_speed_limit() const {
		return speed_limit;
	}

private:
	double collision_threshold_s;
	double speed_mps;
	double speed_limit;
};

TEST_CASE("AutoBrake") {
	MockRepository mocks;
	mocks.autoExpect = false;
	CarDetectedCallback car_detected_callback;
	SpeedUpdateCallback speed_update_callback;
	SpeedLimitCallback speed_limit_callback;
	auto* bus = mocks.Mock<IServiceBus>();
	mocks.ExpectCall(bus, IServiceBus::subscribe_to_speed)
		.Do([&](const auto& x) {
		speed_update_callback = x;
			});
	mocks.ExpectCall(bus, IServiceBus::subscribe_to_car_detected)
		.Do([&](const auto& x) {
		car_detected_callback = x;
			});
	mocks.ExpectCall(bus, IServiceBus::subscribe_to_speed_limit)
		.Do([&](const auto& x) {
		speed_limit_callback = x;
			});
	AutoBrake auto_brake{ *bus };
	SECTION("initializes speed to zero") {
		REQUIRE(auto_brake.get_speed_mps() == Approx(0));
	}
	SECTION("initializes sensitivity to five") {
		REQUIRE(auto_brake.get_collision_threshold_s() == Approx(5));
	}
	SECTION("throws when sensitivity less than one") {
		REQUIRE_THROWS(auto_brake.set_collision_threshold_s(0.5L));
	}
	SECTION("saves speed after update") {
		speed_update_callback(SpeedUpdate{ 30L });
		REQUIRE(30L == auto_brake.get_speed_mps());
		speed_update_callback(SpeedUpdate{ 15L });
		REQUIRE(15L == auto_brake.get_speed_mps());
		speed_update_callback(SpeedUpdate{ 0L });
		REQUIRE(0L == auto_brake.get_speed_mps());
	}
	SECTION("no alert when not imminent") {
		auto_brake.set_collision_threshold_s(2L);
		speed_update_callback(SpeedUpdate{ 10L });
		car_detected_callback(CarDetected{ 1000L, 50L });
	}
	SECTION("alert when imminent") {
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd) {
			return cmd.time_to_collision_s == Approx(1);
				});
		auto_brake.set_collision_threshold_s(10L);
		speed_update_callback(SpeedUpdate{ 10L });
		car_detected_callback(CarDetected{ 10L, 0L });
	}
	SECTION("initializes speed limit to 39") {
		REQUIRE(auto_brake.get_speed_limit() == Approx(39));
	}
	SECTION("saves speed limit after update") {
		speed_limit_callback(SpeedLimitDetected{ 50L });
		REQUIRE(50L == auto_brake.get_speed_limit());
		speed_limit_callback(SpeedLimitDetected{ 25L });
		REQUIRE(25L == auto_brake.get_speed_limit());
		speed_limit_callback(SpeedLimitDetected{ 10L });
		REQUIRE(10L == auto_brake.get_speed_limit());
	}
	SECTION("no alert when under speed limit") {
		speed_limit_callback(SpeedLimitDetected{ 35L });
		speed_update_callback(SpeedUpdate{ 34L });
	}
	SECTION("alert when over speed limit") {
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd) {
			return cmd.time_to_collision_s == Approx(0);
				});
		speed_limit_callback(SpeedLimitDetected{ 35L });
		speed_update_callback(SpeedUpdate{ 40L });
	}
	SECTION("alert when over speed limit after speed limit changed") {
		mocks.ExpectCall(bus, IServiceBus::publish)
			.Match([](const auto& cmd) {
			return cmd.time_to_collision_s == Approx(0);
				});
		speed_limit_callback(SpeedLimitDetected{ 35L });
		speed_update_callback(SpeedUpdate{ 30L });
		speed_limit_callback(SpeedLimitDetected{ 25L });
	}
}