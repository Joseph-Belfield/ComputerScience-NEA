// Element enum, ties atomic number and element name. 
enum Name
{
    HYDROGEN = 1,
    HELIUM,
    LITHIUM,
    BERYLLIUM,
    BORON,
    PHOSPHORUS,
    CARBON,
    NITROGEN,
    OXYGEN,
    FLUORINE,
    NEON
};

// Holds important information about each element. 
struct Element
{
    const Name name;
    float neutrons;
    char periodicBlock;
};

#define HYDROGEN Element(Name::HYDROGEN, 0, 'S')
#define HELIUM Element(Name::HELIUM, 2, 'S')

// Atom class, holds essential details for atoms. 
class Atom 
{
public: 
    const Name element;
    float neutrons;
    int electrons;
    Atom** bonds;

    // Constructor class. 
    Atom(const Element element) : bonds(new Atom* [element.name]),  // Assigns a pointer to an array of pointers to Atoms for bonds
                                  element(element.name),            // Assigns an enum that represents name/protons of an element
                                  neutrons(element.neutrons),       // Assigns float for default number of neutrons
                                  electrons(element.name) {};       // Assigns int for default of electrons

                                
    // Calculates difference between protons and electrons (ie: charge)
    int calculateCharge()
    {
        return (this -> element) - (this -> electrons);
    }

};

