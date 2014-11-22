#include <string>
#include <vector>
#include <exception>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include <cppunit/TestResultCollector.h>
#include <cppunit/CompilerOutputter.h>
#include <cppunit/TextTestProgressListener.h>
#include <cppunit/XmlOutputter.h>
#include <cppunit/TextOutputter.h>
#include <fstream>

class Tester
{
public:
  Tester(const std::vector<std::string>& arg)
    : _testToExecute(""), _xmlOutName(""), _progress(true)
  {
    std::string xmlOpt("--xml-file=");

    for (auto it = arg.begin(), end = arg.end(); it != end; it++)
      {
        if (!it->compare(0, xmlOpt.size(), xmlOpt))
          {
            _xmlOutName = it->substr(xmlOpt.size());
          }
        else
          _testToExecute = *it;
      }
  };

  ~Tester() {};

  void print_all_tests_target(CppUnit::Test *t, const char *spacer, std::ostream& os)
  {
    os << t->getName() << spacer;
    for (int i = 0; i < t->getChildTestCount(); i++)
      print_all_tests_target(t->getChildTestAt(i), spacer, os);
  }

  void print_down_tests_target(CppUnit::Test *t, const char *spacer, std::ostream& os)
  {
    if (t->getChildTestCount() == 0)
      os << t->getName() << spacer;
    for (int i = 0; i < t->getChildTestCount(); i++)
      print_down_tests_target(t->getChildTestAt(i), spacer, os);
  }

  bool run()
  {
    CppUnit::TestResult controller;
    CppUnit::TestResultCollector result;
    CppUnit::TextTestProgressListener progress;
    if (_progress)
      controller.addListener(&progress);
    controller.addListener(&result);

    CppUnit::TextUi::TestRunner runner;
    CppUnit::TestFactoryRegistry& registry = CppUnit::TestFactoryRegistry::getRegistry();
    CppUnit::Test* testToRun = registry.makeTest();

    if(_testToExecute.size() > 0)
      {
        try {
            testToRun = testToRun->findTest(_testToExecute);
          }
        catch (...) {
            std::cerr << _testToExecute << " isn't a valid test name." << std::endl;
            std::cerr << "Valid tests are: " << std::endl;
            print_all_tests_target(testToRun, "\n", std::cerr);
            throw;
          }
      }

    runner.addTest(testToRun);
    runner.run(controller);

    if (_xmlOutName.size() > 0)
      {
        std::ofstream xmlFileOut(_xmlOutName);
        CppUnit::XmlOutputter xmlOut(&result, xmlFileOut);
        xmlOut.write();
        xmlFileOut.close();
      }

    CppUnit::CompilerOutputter textOutput(&result, std::cerr);
    textOutput.write();
    return result.wasSuccessful();
  };

private:
  std::string _testToExecute;
  std::string _xmlOutName;
  bool _progress;
};

int main(int argc, char* argv[])
{
  bool success = false;
  std::vector<std::string> arg;

  try {
  	if (argc > 0)
      arg.reserve(argc - 1);
      for(int i = 1; i < argc; i++)
        arg.push_back(argv[i]);
      Tester tester(arg);

      success = tester.run();
    }
  catch (std::exception& e) {
      std::cerr << e.what() << std::endl;
    }
  return success ? 0 : 1;
}
