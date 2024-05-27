#include "stdafx.h"
#include "Params.hpp"
using namespace RAM_2022_ClMsg;
using namespace std;

using uint = uint32_t;

namespace RAM_2022_Params
{
    void setParams(Params& params, std::span<wchar_t*> mainArgs, ClMsgArrs& clMsgArrs)
    {
        const array<wstring*, PARAMS_NUMBER> P_resultParams{ &params.in, &params.out, &params.log };

        for (size_t iParam = 1; iParam < mainArgs.size(); iParam++)
        {
            wstring_view param{ mainArgs[iParam] };

            for (uint iParamOption = 0; iParamOption < PARAMS_NUMBER; iParamOption++)
            {
                if (!param.starts_with(OPTIONS[iParamOption]) ||
                    param == OPTIONS[iParamOption])
                {
                    continue;
                }

                if (param.length() > PARAM_MAX_LENGTH + OPTIONS[iParamOption].length())
                {
                    clMsgArrs.push(Warning(ClMsgType::PARAMETER_MAX_LENGTH_EXCEEDED,
                        "Превышена длина входного параметра (" + to_string(iParamOption) + ")"));
                    param = param.substr(0, PARAM_MAX_LENGTH + OPTIONS[iParamOption].length());
                }

                *P_resultParams[iParamOption] = param.substr(OPTIONS[iParamOption].length());
                break;
            }
        }

        if (params.in.empty()) {
            throw Error(ClMsgType::MISSING_PARAMETER_IN);
        }

        for (uint iParamOption = 0; iParamOption < PARAMS_NUMBER; iParamOption++)
        {
            if (!P_resultParams[iParamOption]->empty()) {
                continue;
            }

            wstring param = params.in;
            if (param.length() + OPTION_DEFAULT_EXTENSIONS[iParamOption].length() > PARAM_MAX_LENGTH)
            {
                clMsgArrs.push(Warning(ClMsgType::PARAMETER_MAX_LENGTH_EXCEEDED,
                    "Превышена длина автоматически определённого входного параметра (" + to_string(iParamOption) + ")"));
                param = param.substr(0, PARAM_MAX_LENGTH - OPTION_DEFAULT_EXTENSIONS[iParamOption].length());
            }
            param += OPTION_DEFAULT_EXTENSIONS[iParamOption];

            *P_resultParams[iParamOption] = param;
        }
    }
}