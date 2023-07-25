#pragma once

enum class CameraID :i8 {
	Null = -1,
	PLAY,
	DEBUG,
};

#define ICameraID (i8)CameraID