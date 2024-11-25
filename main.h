#pragma once
template <typename T>
void fifo(const std::vector<T> &data, int frameSize);
template <typename T>
void lru(std::vector<T> &data, int frameSize);
template <typename T>
void mru(const std::vector<T> &data, int frameSize);
template <typename T>
void optimal(const std::vector<T> &data, int frameSize);