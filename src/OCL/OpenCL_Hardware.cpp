
#include "OCL/OpenCL_Hardware.h"




cl::Platform OCL_Hardware::loadPlatform()
{
	std::vector<cl::Platform> all_platforms;
	cl::Platform::get(&all_platforms);
	if (all_platforms.size() == 0) {
		std::cout << " No OpenCL platforms found. Check OpenCL installation!\n";
		std::cin.get();
		exit(1);
	}

	for (auto itr : all_platforms)
	{
		static int count = 1;
		std::cout << "Platform [" << count++ << "]: " << itr.getInfo<CL_PLATFORM_NAME>() << std::endl;
	}
	cl::Platform platform;

	while (true)
	{
		std::cout << "Select platform: ";
		std::string str;
		std::getline(std::cin, str);
		std::stringstream sstr(str);
		int id = -1;
		sstr >> id;

		if (id > 0 && id <= all_platforms.size())
		{
			platform = all_platforms[id - 1];
			break;
		}
		else
			std::cout << "Selection wrong: " << id << "\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	}

	std::cout << "\n >> Using platform: " << platform.getInfo<CL_PLATFORM_NAME>() << "\n\n\n";

	return platform;
}

cl::Device OCL_Hardware::loadDevice()
{

	//Get Devices
	std::vector<cl::Device> all_devices;
	m_platform.getDevices(CL_DEVICE_TYPE_ALL, &all_devices);
	if (all_devices.size() == 0) {
		std::cout << " No OpenCL devices found. Check OpenCL installation!\n";
		std::cin.get();
		exit(1);
	}


	std::cout << "Needed Extension: cl_khr_gl_sharing" << std::endl;
	for (auto itr : all_devices)
	{
		static int count = 1;
		std::cout << "Devices [" << count++ << "]: " << itr.getInfo<CL_DEVICE_NAME>() << std::endl;
		std::cout << "Extensions cl_khr_gl_sharing: " << std::boolalpha << (itr.getInfo<CL_DEVICE_EXTENSIONS>().find("cl_khr_gl_sharing") != std::string::npos) << std::endl;
	}


	cl::Device device;
	while (true)
	{
		std::cout << "Select device: ";
		std::string str;
		std::getline(std::cin, str);
		std::stringstream sstr(str);
		int id = -1;
		sstr >> id;

		if (id > 0 && id <= all_devices.size())
		{
			device = all_devices[id - 1];
			break;
		}
		else
			std::cout << "Selection wrong: " << id << "\n\n\n\n\n\n\n\n\n\n\n\n" << std::endl;
	}


	if (device.getInfo<CL_DEVICE_EXTENSIONS>().find("cl_khr_gl_sharing") == std::string::npos)
	{
		std::cout << "\n\n Extension cl_khr_gl_sharing needed but not available!" << std::endl;
		std::cout << "Currently no fallback modus implemented ..." << std::endl;
		std::cin.get();
		exit(-1);
	}
	else
	{
		std::cout << "\nUsing device: " << device.getInfo<CL_DEVICE_NAME>() << "\n\n\n\n";
	}


	std::cout << "Compute Units: " << device.getInfo<CL_DEVICE_MAX_COMPUTE_UNITS>() << std::endl;
	std::cout << "Workgrpoup Size: " << device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>() << std::endl;
	std::cout << "Workitem Dim: " << device.getInfo<CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS>() << std::endl; ;

	std::cout << "Local Mem: " << device.getInfo<CL_DEVICE_LOCAL_MEM_SIZE>() << std::endl;
	std::cout << "Global Mem: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_SIZE>() << std::endl;
	std::cout << "Global Cache Mem: " << device.getInfo<CL_DEVICE_GLOBAL_MEM_CACHE_SIZE>() << std::endl;

	return device;
}


cl::Context OCL_Hardware::loadContext(cl_context_properties dc, cl_context_properties rc)
{

	cl_context_properties props[] =
	{
		CL_GL_CONTEXT_KHR, rc,
		CL_WGL_HDC_KHR, dc,
		CL_CONTEXT_PLATFORM, (cl_context_properties)(m_platform)(),
		0
	};


	cl_int error;
	m_context = cl::Context(m_device, props, 0, 0, &error);
	errorCheck("context creation", error);

	return m_context;
}



OCL_Hardware::OCL_Hardware()
{
	m_platform = loadPlatform();

	m_device = loadDevice();


#ifdef _WIN32
	auto hDC = wglGetCurrentDC();
	auto hRC = wglGetCurrentContext();
#else
	auto hDC = glXGetCurrentContext();
	auto hRC = glXGetCurrentDisplay();
#endif


	m_context = loadContext(reinterpret_cast<cl_context_properties>(hDC), reinterpret_cast<cl_context_properties>(hRC));
}