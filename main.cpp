#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include "main.h"

int main(int argc, char const *argv[])
{
    int frames = 0;
    int pages = 0;
    int pageFaults = 0;
    int pageSize = 0;

   /*  if (argc == 2)
    {
        pageSize = std::stoi(argv[1]);
        frames = std::stoi(argv[2]);
    } else
    {
        std::cerr << "wrong number of arguments\n";
        exit(1);
    } */

    std::vector<int> data;
    std::ifstream inFile("data.txt");

    if (inFile.is_open())
    {   int temp = 0;
        data.reserve(3000);
        while (inFile >> temp)
        {
            data.emplace_back(temp);
        }
        inFile.close();
    }else
    {
        std::cerr << "unable to open file\n";
    }

    std::vector<int> v512,v1024,v2048;
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

    
    
    return 0;
}
//pages, frame size
void FIFO(const std::vector<int> &data, int frameSize)
{
    int pageFaults = 0;
    std::vector<int> activePages;
    std::queue<int> fifoQueue;
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
