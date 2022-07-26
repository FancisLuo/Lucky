
#include <Lucky.h>

class Sandbox : public Lucky::Application
{
public:
	Sandbox() {}

	~Sandbox() {}
};

void main()
{
	Sandbox* sandbox = new Sandbox();
	sandbox->Run();
	delete sandbox;
}