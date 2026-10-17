#include <Razor.h>
#include <Razor/EntryPoint.h>




class Sandbox : public Razor::Application
{
public:
	Sandbox() {};
	~Sandbox() {};
	void Run() override;
};

Razor::Application* Razor::CreateApplication()
{
	return new Sandbox();
}



void Sandbox::Run()
{
	
	
}


