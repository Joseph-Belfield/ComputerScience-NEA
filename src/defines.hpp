#include <iostream>
#include <stack>
#include <vector>
using namespace std;

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
    int atomID;
    const Name element;
    float neutrons;
    int electrons;
    Atom** bonds;   // pointer to array of pointers to other atom objects

    // Constructor class. This assigns the default value of the attributes.
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

// A class for effectively anything rendered - functional groups will be a subclass
class Molecule
{
private:
    const unsigned int moleculeID;
    unsigned int nextID = 0;
    unsigned int atomCounter = 0;
    vector<Atom*>* atoms_vector;

public: 
    string name = "";
    Atom* root = nullptr;

    // Constructor class. This assigns the default value of the attributes.
    Molecule(const unsigned int moleculeID) : moleculeID(moleculeID) {};

    // Creates a 'molecule' out of 1 atom - only run once per molecule
    void addAtom(Atom* atom)
    {
        atom -> atomID = this -> nextID++;  // sets atom's ID to next availible ID, then increments
        root = atom;                        // sets root of molecule to atom
        atomCounter++;                      // Increases the number of atoms in molecule counter by 1
        atoms_vector -> push_back(atom);    // Add's atom pointer to end of vector of atom pointers
    }

    // Joins specified atom of this molecule to specified atom of other molecule
    void addMolecule(Molecule* joinedMolecule, Atom* sourceAtomPtr, Atom* destinationAtomPtr)
    {
        for (int i = 0; i < (joinedMolecule -> atoms_vector -> size()); i++)
        {

        }
    }

    // Adds another bond between two atoms within the same molecule
    void addBond(unsigned int atomID_1, unsigned int atomID_2)
    {

    }

};
