#pragma once


#define OBSERVED(type, name, value) Observed<type> name(value, #name);
