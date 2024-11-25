#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
#include <limits>

#include "main.h"

int main(int argc, char const *argv[])
{
 
    std::ifstream inFile("data.txt", std::ios::binary);
    if (!inFile.is_open())
    {   
        std::cerr << "unable to open file\n";
        exit(1);
    }

    // Get file size
    inFile.seekg(0, std::ios::end);
    std::streampos fileSize = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    // Create vector to store bytes
    std::vector<unsigned char> data(fileSize);

    // Read file into vector
    inFile.read((char*)(&data[0]), fileSize);
    inFile.close();

    std::vector<unsigned char> v512,v1024,v2048;
    
    v512.assign(data.begin(), data.begin()+512);
    v1024.assign(data.begin(), data.begin()+1024);
    v2048.assign(data.begin(), data.begin()+2048);
    
    fifo(v512, 4);
    fifo(v512, 8);
    fifo(v512, 12);
    fifo(v1024, 4);
    fifo(v1024, 8);
    fifo(v1024, 12);
    fifo(v2048, 4);
    fifo(v2048, 8);
    fifo(v2048, 12);

    lru(v512, 4);
    lru(v512, 8);
    lru(v512, 12);
    lru(v1024, 4);
    lru(v1024, 8);
    lru(v1024, 12);
    lru(v2048, 4);
    lru(v2048, 8);
    lru(v2048, 12);

    mru(v512, 4);
    mru(v512, 8);
    mru(v512, 12);
    mru(v1024, 4);
    mru(v1024, 8);
    mru(v1024, 12);
    mru(v2048, 4);
    mru(v2048, 8);
    mru(v2048, 12);

    optimal(v512, 4);
    optimal(v512, 8);
    optimal(v512, 12);
    optimal(v1024, 4);
    optimal(v1024, 8);
    optimal(v1024, 12);
    optimal(v2048, 4);
    optimal(v2048, 8);
    optimal(v2048, 12);

    return 0;
}
template <typename T>

void optimal(const std::vector<T> &data, int frameSize)
{
    int pageFaults = 0;
    std::vector<T> activePages;
    
    for (int i = 0; i < data.size(); i++)
    {
        auto currentAddr = std::find(activePages.begin(), activePages.end(), data[i]);
        if (activePages.size() < frameSize)
        {
            if (currentAddr == activePages.end())
            {
                activePages.push_back(data[i]);
                pageFaults++;
            }
        }
        else
        {
            if (currentAddr == activePages.end())
            {
                pageFaults++;
                typename std::vector<T>::const_iterator farthest = data.begin()+i;
                typename std::vector<T>::const_iterator checking;
                for (int j = 0; j < frameSize; j++)
                {
                    checking = std::find(data.begin()+i,data.end(), activePages[j]);
                    if (checking > farthest)
                    {
                        farthest = checking;
                    }
                }
                std::replace(activePages.begin(), activePages.end(), *farthest, data[i]);
            }
        }
    }
    std::cout << "Page Size\t#of frames\tPage replacement ALG\tPage fault percentage\n";
    std::cout << data.size() << "\t\t" << frameSize << "\t\t" << "optimal\t\t\t" << ((double)pageFaults / (double)data.size())*100 << "\n";
}

template <typename T>
void mru(const std::vector<T> &data, int frameSize)
{
    std::unordered_map<T, int> currentAddrsMap;
    std::vector<T> activePages;
    int pageFaults = 0;
    for (int i = 0; i < data.size(); i++)
    {
        auto currentAddr = std::find(activePages.begin(), activePages.end(), data[i]);
        if (activePages.size() < frameSize)
        {
            if (currentAddr == activePages.end())
            {
                activePages.push_back(data[i]);
                pageFaults++;
            }
            currentAddrsMap[data[i]] = i;
        }
        else
        {
            if (currentAddr == activePages.end())
            {
                int max = 0;
                T maxKey;

                for (const auto &j : activePages)
                {
                    if (currentAddrsMap[j] > max)
                    {
                        max = currentAddrsMap[j];
                        maxKey = j;
                    }
                }
                std::replace(activePages.begin(), activePages.end(), maxKey, data[i]);
                pageFaults++;
            }
            currentAddrsMap[data[i]] = i;
        }
    }
    std::cout << "Page Size\t#of frames\tPage replacement ALG\tPage fault percentage\n";
    std::cout << data.size() << "\t\t" << frameSize << "\t\t" << "mru\t\t\t" << ((double)pageFaults / (double)data.size())*100 << "\n";
}

template <typename T>
void lru(std::vector<T> &data, int frameSize)
{
    std::unordered_map<T, int> currentAddrsMap;
    std::vector<T> activePages;
    int pageFaults = 0;
    for (int i = 0; i < data.size(); i++)
    {
        auto currentAddr = std::find(activePages.begin(), activePages.end(), data[i]);
        if (activePages.size() < frameSize)
        {
            if (currentAddr == activePages.end())
            {
                activePages.push_back(data[i]);
                pageFaults++;
            }
            currentAddrsMap[data[i]] = i;
        }
        else
        {
            if (currentAddr == activePages.end())
            {
                int min = std::numeric_limits<int>::max();
                T minKey;

                for (const auto &j : activePages)
                {
                    if (currentAddrsMap[j] < min)
                    {
                        min = currentAddrsMap[j];
                        minKey = j;
                    }
                }
                std::replace(activePages.begin(), activePages.end(), minKey, data[i]);
                pageFaults++;
            }
            currentAddrsMap[data[i]] = i;
        }
    }
    std::cout << "Page Size\t#of frames\tPage replacement ALG\tPage fault percentage\n";
    std::cout << data.size() << "\t\t" << frameSize << "\t\t" << "LRU\t\t\t" << ((double)pageFaults / (double)data.size())*100 << "\n";
}

template <typename T>
//pages, frame size
void fifo(const std::vector<T> &data, int frameSize)
{
    int pageFaults = 0;
    std::vector<T> activePages;
    std::queue<T> fifoQueue;
    for (auto add : data)
    {
        auto currentAdd = std::find(activePages.begin(), activePages.end(), add);
        if (activePages.size() < frameSize)
        {
            if (currentAdd == activePages.end())
            {
                activePages.push_back(add);
                pageFaults++;
                fifoQueue.push(add);
            }
        }
        else
        {
            if (currentAdd == activePages.end())
            {
                std::replace(activePages.begin(), activePages.end(), fifoQueue.front(), add);
                fifoQueue.pop();
                fifoQueue.push(add);
                pageFaults++;
            }
        }
    }
    std::cout << "Page Size\t#of frames\tPage replacement ALG\tPage fault percentage\n";
    std::cout << data.size() << "\t\t" << frameSize << "\t\t" << "FIFO\t\t\t" << ((double)pageFaults / (double)data.size())*100 << "\n";
}
