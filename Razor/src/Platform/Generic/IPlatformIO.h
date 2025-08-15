#pragma once
class IPlatformIO
{
public:
	virtual void RegisterInputCallbacks() = 0;
	virtual ~IPlatformIO() = default;
};

