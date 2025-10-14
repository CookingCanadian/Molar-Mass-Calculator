#include "element_data.h"
#include <algorithm>
#include <cctype>
#include <cstdlib>


struct MoleculeComponent {
    std::string element;
    int count;
};

double CalculateMolarMass(const std::string& formula) {
    std::vector<MoleculeComponent> components;
    int i = 0;
    int leadingMultiplier = 1;
    
    // parse leading number
    if (std::isdigit(formula[0])) {
        leadingMultiplier = 0;
        while (i < formula.length() && std::isdigit(formula[i])) {
            leadingMultiplier = leadingMultiplier * 10 + (formula[i] - '0');
            i++;
        }
    }
    
    // parse element symbols and their counts
    while (i < formula.length()) {
        // skip whitespace
        if (std::isspace(formula[i])) {
            i++;
            continue;
        }
    
        // parse element symbol
        if (std::isupper(formula[i])) {
            std::string element;
            element += formula[i];
            i++;
            
            while (i < formula.length() && std::islower(formula[i])) {
                element += formula[i];
                i++;
            }            

            int count = 1;
            if (i < formula.length() && (std::isdigit(formula[i]) || (i + 1 < formula.length() && formula[i] == '_' && std::isdigit(formula[i + 1])))) {             
                count = 0;
                while (i < formula.length() && std::isdigit(formula[i])) {
                    count = count * 10 + (formula[i] - '0');
                    i++;
                }
            }
            
            components.push_back({element, count});
        } else {
            i++; // skip unknown characters
        }
    }
    
    // calculate total molar mass
    double totalMass = 0.0;
    
    for (const auto& comp : components) {
        const Element* elem = FindElementBySymbol(comp.element);
        if (elem) {
            totalMass += elem->molarMass * comp.count * leadingMultiplier;
        } else {
            printf("Warning: Element '%s' not found\n", comp.element.c_str());
        }
    }
    
    return totalMass;
}

const std::vector<Element>& GetPeriodicTable() {
    static std::vector<Element> periodicTable = {
        {1,   "H",  "Hydrogen",       1.00784},
        {2,   "He", "Helium",         4.002602},
        {3,   "Li", "Lithium",        6.941},
        {4,   "Be", "Beryllium",      9.0121831},
        {5,   "B",  "Boron",          10.811},
        {6,   "C",  "Carbon",         12.011},
        {7,   "N",  "Nitrogen",       14.007},
        {8,   "O",  "Oxygen",         15.999},
        {9,   "F",  "Fluorine",       18.998403162},
        {10,  "Ne", "Neon",           20.1797},
        {11,  "Na", "Sodium",         22.98976928},
        {12,  "Mg", "Magnesium",      24.305},
        {13,  "Al", "Aluminum",       26.9815384},
        {14,  "Si", "Silicon",        28.0855},
        {15,  "P",  "Phosphorus",     30.973762},
        {16,  "S",  "Sulfur",         32.065},
        {17,  "Cl", "Chlorine",       35.453},
        {18,  "Ar", "Argon",          39.948},
        {19,  "K",  "Potassium",      39.0983},
        {20,  "Ca", "Calcium",        40.078},
        {21,  "Sc", "Scandium",       44.955912},
        {22,  "Ti", "Titanium",       47.867},
        {23,  "V",  "Vanadium",       50.9415},
        {24,  "Cr", "Chromium",       51.9961},
        {25,  "Mn", "Manganese",      54.938044},
        {26,  "Fe", "Iron",           55.845},
        {27,  "Co", "Cobalt",         58.933195},
        {28,  "Ni", "Nickel",         58.6934},
        {29,  "Cu", "Copper",         63.546},
        {30,  "Zn", "Zinc",           65.38},
        {31,  "Ga", "Gallium",        69.723},
        {32,  "Ge", "Germanium",      72.64},   
        {33,  "As", "Arsenic",        74.9216},
        {34,  "Se", "Selenium",       78.971},
        {35,  "Br", "Bromine",        79.904},
        {36,  "Kr", "Krypton",        83.798},
        {37,  "Rb", "Rubidium",       85.4678},
        {38,  "Sr", "Strontium",      87.62},
        {39,  "Y",  "Yttrium",        88.90585},
        {40,  "Zr", "Zirconium",      91.224},
        {41,  "Nb", "Niobium",        92.90638},
        {42,  "Mo", "Molybdenum",     95.95},
        {43,  "Tc", "Technetium",     98.0},      
        {44,  "Ru", "Ruthenium",     101.07},
        {45,  "Rh", "Rhodium",       102.9055},
        {46,  "Pd", "Palladium",     106.42},
        {47,  "Ag", "Silver",        107.8682},
        {48,  "Cd", "Cadmium",       112.414},
        {49,  "In", "Indium",        114.818},
        {50,  "Sn", "Tin",           118.710},
        {51,  "Sb", "Antimony",      121.760},
        {52,  "Te", "Tellurium",     127.60},
        {53,  "I",  "Iodine",        126.90447},
        {54,  "Xe", "Xenon",         131.293},
        {55,  "Cs", "Cesium",        132.9054519}, // Caesium (international) or Cesium (American)
        {56,  "Ba", "Barium",        137.327},
        {57,  "La", "Lanthanum",     138.90547},
        {58,  "Ce", "Cerium",        140.116},
        {59,  "Pr", "Praseodymium",  140.90765},
        {60,  "Nd", "Neodymium",     144.242},
        {61,  "Pm", "Promethium",    145.0},    
        {62,  "Sm", "Samarium",      150.36},
        {63,  "Eu", "Europium",      151.964},
        {64,  "Gd", "Gadolinium",    157.25},
        {65,  "Tb", "Terbium",       158.92535},
        {66,  "Dy", "Dysprosium",    162.500},
        {67,  "Ho", "Holmium",       164.93032},
        {68,  "Er", "Erbium",        167.259},
        {69,  "Tm", "Thulium",       168.93421},
        {70,  "Yb", "Ytterbium",     173.045},
        {71,  "Lu", "Lutetium",      174.9668},
        {72,  "Hf", "Hafnium",       178.486},
        {73,  "Ta", "Tantalum",      180.94788},
        {74,  "W",  "Tungsten",      183.84},
        {75,  "Re", "Rhenium",       186.207},
        {76,  "Os", "Osmium",        190.23},
        {77,  "Ir", "Iridium",       192.217},
        {78,  "Pt", "Platinum",      195.084},
        {79,  "Au", "Gold",          196.966570},
        {80,  "Hg", "Mercury",       200.59},
        {81,  "Tl", "Thallium",      204.3833},
        {82,  "Pb", "Lead",          207.2},     
        {83,  "Bi", "Bismuth",       208.98040},
        {84,  "Po", "Polonium",      208.98243},    
        {85,  "At", "Astatine",      209.98715},     
        {86,  "Rn", "Radon",         222.01758},     
        {87,  "Fr", "Francium",      223.01973},    
        {88,  "Ra", "Radium",        226.02541},
        {89,  "Ac", "Actinium",      227.02775},
        {90,  "Th", "Thorium",       232.0377},
        {91,  "Pa", "Protactinium",  231.03588},
        {92,  "U",  "Uranium",       238.0289},
        {93,  "Np", "Neptunium",     237.048172},
        {94,  "Pu", "Plutonium",     244.06420},
        {95,  "Am", "Americium",     243.061380},
        {96,  "Cm", "Curium",        247.07035},
        {97,  "Bk", "Berkelium",     247.07031},
        {98,  "Cf", "Californium",   251.07959},
        {99,  "Es", "Einsteinium",   252.0830},
        {100, "Fm", "Fermium",       257.09511},
        {101, "Md", "Mendelevium",   258.09843},
        {102, "No", "Nobelium",      259.10100},
        {103, "Lr", "Lawrencium",    266.120},
        {104, "Rf", "Rutherfordium", 267.122},
        {105, "Db", "Dubnium",       268.126},
        {106, "Sg", "Seaborgium",    269.128},
        {107, "Bh", "Bohrium",       270.133},
        {108, "Hs", "Hassium",       269.1336},
        {109, "Mt", "Meitnerium",    277.154},
        {110, "Ds", "Darmstadtium",  282.166},
        {111, "Rg", "Roentgenium",   282.169},
        {112, "Cn", "Copernicium",   286.179},
        {113, "Nh", "Nihonium",      286.182},
        {114, "Fl", "Flerovium",     290.192},
        {115, "Mc", "Moscovium",     290.196},
        {116, "Lv", "Livermorium",   293.205},
        {117, "Ts", "Tennessine",    294.211},
        {118, "Og", "Oganesson",     295.216}
    };
    return periodicTable;
}

const Element* FindElementBySymbol(const std::string& symbol) {
    const auto& table = GetPeriodicTable();
    auto it = std::find_if(table.begin(), table.end(),
        [&](const Element& e) { return e.symbol == symbol; });
    return (it != table.end()) ? &(*it) : nullptr;
}
