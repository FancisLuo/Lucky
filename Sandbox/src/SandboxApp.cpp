
#include <Lucky.h>

class Sandbox : public Lucky::Application
{
public:
	Sandbox() {}

	~Sandbox() {}
};

int main(int argc, char** argv)
{
	Lucky::LuckyLog::Init();

	//Lucky::LuckyLog::GetCoreLogger()->warn("initialized!!");
	LK_CORE_WARN("Initialized!!!");

	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}