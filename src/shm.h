#pragma once

#include <cstddef>

void* ShmCreate(const char* name, std::size_t size);

void* ShmAttach(const char* name, std::size_t size);

bool ShmClose(void* addr, std::size_t size);

bool ShmDestroy(const char* name);
