#include <Rcpp.h>
#include <string>
#include <list>
using namespace Rcpp;
// [[Rcpp::plugins(cpp11)]]


//' Interprets text and stores it in a data frame.
//'
//' @param lines The text of the results to convert.
//' @param dfResults A prebuilt data frame to store results into
//' @export
// [[Rcpp::export]]
void convertPAMS(StringVector lines, List dfResults) {
    // Order: Place, Name, Grade, School, Time
    NumericVector places = dfResults["Place"];
    StringVector names = dfResults["Name"];
    NumericVector grades = dfResults["Grade"];
    StringVector schools = dfResults["School"];
    StringVector times = dfResults["Time"];

    int row = 0;
    for(int i = 0; i < lines.size(); i++)
    {
        // Initialize the string for this runner's line
        std::string line = std::string(lines[i]);

        // Skip over the line if not a runner
        if(line.size() <= 1)
            continue;

        std::list<std::string> lineList {};
        std::string current = "";

        for(unsigned int posInLine = 0; posInLine < line.length(); posInLine++) {
            // If there are characters to add
            if(line[posInLine] == ' ') {
                if(!current.empty()) {
                    lineList.push_back(current);
                    current = "";
                }
            } else {
                current += line[posInLine];
            }
        }
        lineList.pop_back(); // Removes TeamPlace
        // lineList now contains
        // [Place, First Name, Last Name, Grade, School ..., Time]
        // The items in parenthesis might not be present.

    // Place
        // std::istringstream(lineList.front()) >> places[row];
        places[row] = std::stoi(lineList.front());
        lineList.pop_front();


    // Name
        std::string name = lineList.front();
        lineList.pop_front();
        // until the next character is a number (for grade)
        while(!isdigit(lineList.front()[0]))
        {
            name += ' ';
            name += lineList.front();
            lineList.pop_front();
        }
        names[row] = name;


    // Grade
        grades[row] = std::stoi(lineList.front());
        lineList.pop_front();

    // Time
        // Skip to time at the end because it's easy
        times[row] = lineList.back();
        lineList.pop_back();

    // School
        std::string schoolName = lineList.front();
        lineList.pop_front();
        // Combine all words in the school name into one string
        while(!isdigit(lineList.front()[0])) {
            schoolName += ' ';
            schoolName += lineList.front();
            lineList.pop_front();
        }
        schools[row] = schoolName;

        row++;
    }
}