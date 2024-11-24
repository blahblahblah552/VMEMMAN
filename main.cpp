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
   /*  if (argc == 2)
    {
        pageSize = std::stoi(argv[1]);
        frames = std::stoi(argv[2]);
    } else
    {
        std::cerr << "wrong number of arguments\n";
        exit(1);
    } */

   
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

    // Read file into a vector of bytes
    std::vector<std::byte> data(fileSize);
    inFile.read(reinterpret_cast<char*>(data.data()), fileSize);

    // Print the bytes
    for (const auto& byte : data) {
        std::cout << std::oct << static_cast<int>(byte) << " ";
    }
    std::cout << std::endl;

    std::vector<std::byte> v512,v1024,v2048;
    v512.assign(data.begin(), data.begin()+512);
    v1024.assign(data.begin(), data.begin()+1024);
    v2048.assign(data.begin(), data.begin()+2048);
    FIFO(v512, 4);
    FIFO(v512, 8);
    FIFO(v512, 12);
    FIFO(v1024, 4);
    FIFO(v1024, 8);
    FIFO(v1024, 12);
    FIFO(v2048, 4);
    FIFO(v2048, 8);
    FIFO(v2048, 12);

    LRU(v512, 4);
    LRU(v512, 8);
    LRU(v512, 12);
    LRU(v1024, 4);
    LRU(v1024, 8);
    LRU(v1024, 12);
    LRU(v2048, 4);
    LRU(v2048, 8);
    LRU(v2048, 12);

    return 0;
}
void LRU(std::vector<std::byte> &data, int frameSize)
{
    std::unordered_map<std::byte, int> currentAddrsMap;
    std::vector<std::byte> activePages;
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
                std::byte minKey;

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
//pages, frame size
void FIFO(const std::vector<std::byte> &data, int frameSize)
{
    int pageFaults = 0;
    std::vector<std::byte> activePages;
    std::queue<std::byte> fifoQueue;
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
