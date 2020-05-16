#include <iostream>
#include <fstream>

#include "Image.h"

static std::vector<std::vector<Image::Pixel>> ReadImageFromCSV(std::ifstream& input)
{
    size_t height, width;
    input >> height >> width;
    std::vector<std::vector<Image::Pixel>> table;
    for (size_t rowId = 0; rowId < height; ++rowId)
    {
        std::vector<Image::Pixel> row;
        for (size_t columnId = 0; columnId < width; ++columnId)
        {
            size_t red, green, blue;
            input >> red >> green >> blue;
            row.emplace_back(red, green, blue);
        }
        table.emplace_back(std::move(row));
    }
    return table;
}

int main(int argc, char* argv[])
{
    // Check command line arguments
    const size_t expectedAmountOfArgs = 2;
    if (argc != expectedAmountOfArgs)
    {
        std::cout << "Wrong amount of arguments. Provide a filename as an argument. See example below:\n";
        std::cout << "seam-carving /data/boat.csv" << std::endl;
        return 0;
    }
    // Check csv file
    std::ifstream inputFile(argv[1]);
    if (!inputFile.good())
    {
        std::cout << "Can't open source file " << argv[1] << ". Verify that the file exists." << std::endl;
    }
    else
    {
        auto imageSource = ReadImageFromCSV(inputFile);
        std::cout << "Success!" << std::endl;
    }
    return 0;
}