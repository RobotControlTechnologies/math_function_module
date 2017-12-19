#include <cstdlib>
#include <exception>
#include <ctime>
#include <cmath>

#include <cstdarg>

#include "module.h"
#include "function_module.h"

#include "math_function_module.h"

#define IID "RCT.Math_functions_module_v107"
const unsigned int COUNT_MATH_FUNCTIONS = 13;

#define ADD_MATH_FUNCTION1(FUNCTION_NAME)                                \
  math_functions[function_id] = new FunctionData;                        \
  math_functions[function_id]->command_index = function_id + 1;          \
  math_functions[function_id]->count_params = 1;                         \
  math_functions[function_id]->params = new FunctionData::ParamTypes[1]; \
  math_functions[function_id]->params[0] = FunctionData::FLOAT;          \
  math_functions[function_id]->name = FUNCTION_NAME;                     \
  function_id++;
#define ADD_MATH_FUNCTION2(FUNCTION_NAME)                                \
  math_functions[function_id] = new FunctionData;                        \
  math_functions[function_id]->command_index = function_id + 1;          \
  math_functions[function_id]->count_params = 2;                         \
  math_functions[function_id]->params = new FunctionData::ParamTypes[2]; \
  math_functions[function_id]->params[0] = FunctionData::FLOAT;          \
  math_functions[function_id]->params[1] = FunctionData::FLOAT;          \
  math_functions[function_id]->name = FUNCTION_NAME;                     \
  function_id++;

#define DEFINE_ALL_FUNCTIONS \
  ADD_MATH_FUNCTION2("pow")  \
  ADD_MATH_FUNCTION1("abs")  \
  ADD_MATH_FUNCTION1("sqrt") \
  ADD_MATH_FUNCTION2("rand") \
  ADD_MATH_FUNCTION1("sin")  \
  ADD_MATH_FUNCTION1("cos")  \
  ADD_MATH_FUNCTION1("tan")  \
  ADD_MATH_FUNCTION1("asin") \
  ADD_MATH_FUNCTION1("acos") \
  ADD_MATH_FUNCTION1("atan") \
  ADD_MATH_FUNCTION1("exp")  \
  ADD_MATH_FUNCTION1("log")  \
  ADD_MATH_FUNCTION1("log10")

MathFunctionModule::MathFunctionModule() {
#if MODULE_API_VERSION > 000
  mi = new ModuleInfo;
  mi->uid = IID;
  mi->mode = ModuleInfo::Modes::PROD;
  mi->version = BUILD_NUMBER;
  mi->digest = NULL;
#endif
  srand(time(NULL));

  math_functions = new FunctionData *[COUNT_MATH_FUNCTIONS];
  system_value function_id = 0;
  DEFINE_ALL_FUNCTIONS
}

#if MODULE_API_VERSION > 000
const struct ModuleInfo &MathFunctionModule::getModuleInfo() { return *mi; }
#else
const char *MathFunctionModule::getUID() { return IID; }
#endif

FunctionData **MathFunctionModule::getFunctions(unsigned int *count_functions) {
  *count_functions = COUNT_MATH_FUNCTIONS;
  return math_functions;
}

void *MathFunctionModule::writePC(unsigned int *buffer_length) {
  *buffer_length = 0;
  return NULL;
}

#if MODULE_API_VERSION > 100
int MathFunctionModule::init() { return 0; }
int MathFunctionModule::readPC(int pc_index, void *buffer, unsigned int buffer_length) { return 0; }
int MathFunctionModule::startProgram(int run_index, int pc_index) { return 0; }
#else
int MathFunctionModule::startProgram(int run_index) { return 0; }
#endif

#if MODULE_API_VERSION > 100
FunctionResult *MathFunctionModule::executeFunction(int run_index, system_value function_index,
                                                    void **args) {
#else
FunctionResult *MathFunctionModule::executeFunction(system_value function_index,
                                                    void **args) {
#endif  
  if ((function_index < 1) || (function_index > ((int)COUNT_MATH_FUNCTIONS))) {
    return NULL;
  }

  try {
    variable_value *input = (variable_value *)(*args);
    variable_value rez = 0;
    switch (function_index) {
      case 1: {
        variable_value *input2 = (variable_value *)(*(args + 1));
        rez = std::pow(*input, *input2);
        break;
      }
      case 2: {
        rez = std::abs(*input);
        break;
      }
      case 3: {
        if ((*input) < 0) {
          throw std::exception();
        }
        rez = std::sqrt(*input);
        break;
      }
      case 4: {
        if ((*input) <= 0) {
          throw std::exception();
        }
        variable_value *input2 = (variable_value *)(*(args + 1));
        rez = (variable_value)(rand() % ((int)(*input)) + ((int)(*input2)));
        break;
      }
      case 5: {
        rez = std::sin(*input);
        break;
      }
      case 6: {
        rez = std::cos(*input);
        break;
      }
      case 7: {
        rez = std::tan(*input);
        break;
      }
      case 8: {
        if (((*input) < -1) || ((*input) > 1)) {
          throw std::exception();
        }
        rez = std::asin(*input);
        break;
      }
      case 9: {
        if (((*input) < -1) || ((*input) > 1)) {
          throw std::exception();
        }
        rez = std::acos(*input);
        break;
      }
      case 10: {
        rez = std::atan(*input);
        break;
      }
      case 11: {
        rez = std::exp(*input);
        break;
      }
      case 12: {
        if (*input <= 0) {
          throw std::exception();
        }
        rez = std::log(*input);
        break;
      }
      case 13: {
        if (*input <= 0) {
          throw std::exception();
        }
        rez = std::log10(*input);
        break;
      }
    }
#if MODULE_API_VERSION > 000
    return new FunctionResult(FunctionResult::Types::VALUE, rez);
#else
    return new FunctionResult(1, rez);
#endif
  } catch (...) {
#if MODULE_API_VERSION > 000
    return new FunctionResult(FunctionResult::Types::EXCEPTION);
#else
    return new FunctionResult(0);
#endif
  }
}

#if MODULE_API_VERSION == 102
  int MathFunctionModule::init(initCallback_t& initCallback) { return init(); }
#endif

int MathFunctionModule::endProgram(int run_index) { return 0; }

void MathFunctionModule::destroy() {
#if MODULE_API_VERSION > 000
  delete mi;
#endif
  for (unsigned int j = 0; j < COUNT_MATH_FUNCTIONS; ++j) {
    if (math_functions[j]->count_params) {
      delete[] math_functions[j]->params;
    }
    delete math_functions[j];
  }
  delete[] math_functions;
  delete this;
}

#if MODULE_API_VERSION > 000
PREFIX_FUNC_DLL unsigned short getFunctionModuleApiVersion() {
  return MODULE_API_VERSION;
}
#endif

PREFIX_FUNC_DLL FunctionModule *getFunctionModuleObject() {
  return new MathFunctionModule();
}
