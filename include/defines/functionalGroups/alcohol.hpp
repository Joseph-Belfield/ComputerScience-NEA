#ifndef ATOM
    #include "atom.hpp"
    #define ATOM
#endif

#ifndef MOLECULE
    #include "molecule.hpp"
    #define MOLECULE
#endif

#ifndef FUNCTIONAL_GROUP
    #include "functionalGroup.hpp"
    #define FUNCTIONAL_GROUP
#endif

#ifndef VECTOR
    #include <vector>
    #define VECTOR
#endif

// alcohol is a child class of functional group
class Alcohol: public Functional_Group
{
public:
    // constiunt atoms should be stored as [O,H]

    Alcohol(Atom* oxygen, Atom* hydrogen)
    {
        constituantAtoms.at(0) = oxygen;
        constituantAtoms.at(1) = hydrogen;
    }

    // verifies if two given atoms are an alcohol group
    bool isAlcohol(Atom* atom1, Atom* atom2)
    {
        Atom* oxygenAtom;
        Atom* hydrogenAtom;
        if (atom1 -> element == OXYGEN && atom2 -> element == HYDROGEN)
        {
            oxygenAtom = atom1;
            hydrogenAtom = atom2;
        }
        else if (atom2 -> element == OXYGEN && atom1 -> element == HYDROGEN)
        {
            oxygenAtom = atom2;
            hydrogenAtom = atom1;
        }
        else
        {
            // if they aren't a hydrogen and oxygen, they cannot be a hydroxyl group
            return false;
        }

        // checks if the oxygen atom and hydrogen atom are bonded (hydrogen should only have 1 bond)
        bool bonded = false;
        for (int i = 0; i < oxygenAtom -> bonds.size(); i++)
        {
            if (oxygenAtom -> bonds.at(i) == hydrogenAtom && hydrogenAtom -> bonds.at(0) == oxygenAtom)
            {
                bonded = true;
            }
        }
        if (bonded == false)
        {
            return false;
        }

        for (int i = 0; i < oxygenAtom -> bonds.size(); i++)
        {
            // if the oxygen atom is bonded to a carbon
            if (oxygenAtom -> bonds.at(i) -> element == CARBON)
            {
                Atom* attatchedCarbon = oxygenAtom -> bonds.at(i);
                Atom* otherOxygen[2] = {nullptr};

                // check through all of carbons bonds for oxygen (except the given atom) - if none of the bonds are a repeated oxygen, return true
                for (int j = 0; j < attatchedCarbon -> bonds.size(); j++)
                {
                    if (attatchedCarbon -> bonds.at(j) == oxygenAtom)
                    {
                        j++;
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && otherOxygen[0] == nullptr )
                    {
                        otherOxygen[0] = attatchedCarbon -> bonds.at(j);
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && attatchedCarbon -> bonds.at(j) != otherOxygen[0] && otherOxygen[1] == nullptr)
                    {
                        otherOxygen[1] = attatchedCarbon -> bonds.at(j);
                    }
                    else if (attatchedCarbon -> bonds.at(j) -> element == OXYGEN && attatchedCarbon -> bonds.at(j) != otherOxygen[0] && attatchedCarbon -> bonds.at(j) != otherOxygen[1])
                    {
                        return true;
                    }
                }
            }
        }

        // if the above conditions are failed, it's false
        return false;
    }

    // finds the first alcohol group it can in a molecule, and returns a pointer to the oxygen of the alcohol group
    Atom* findAlcohol(Molecule* molecule)
    {
        bool alcFound = false;
        for (int i = 0; i < molecule -> atoms.size() && alcFound == false; i++)
        {
            // for each hydrogen in the molecule
            if (molecule -> atoms.at(i) -> element == HYDROGEN)
            {
                // check if its connected to an oxygen
                if (molecule -> atoms.at(i) -> bonds.at(0) -> element == OXYGEN)
                {
                    // check if these atoms make up an alcohol
                    Atom* hydrogen = molecule -> atoms.at(i);
                    Atom* oxygen = molecule -> atoms.at(i) -> bonds.at(0);

                    if (isAlcohol(oxygen, hydrogen))
                    {
                        // if they do make up an alcohol, return the oxygen
                        alcFound = true;
                        return oxygen;
                    }
                }
            }
        }

        // if no alcohols are found, return a nullptr
        return nullptr;
    }

    // finds the first alcohol group it can in a molecule, and returns a pointer to a new alcohol object made of these atoms
    Alcohol* findAlcohol(Molecule* molecule, bool create)
    {
        // if they don't want a return (unsure why) return nullptr
        if (create == false)
        {
            return nullptr;
        }
        
        bool alcFound = false;
        for (int i = 0; i < molecule -> atoms.size() && alcFound == false; i++)
        {
            // for each hydrogen in the molecule
            if (molecule -> atoms.at(i) -> element == HYDROGEN)
            {
                // check if its connected to an oxygen
                if (molecule -> atoms.at(i) -> bonds.at(0) -> element == OXYGEN)
                {
                    // check if these atoms make up an alcohol
                    Atom* hydrogen = molecule -> atoms.at(i);
                    Atom* oxygen = molecule -> atoms.at(i) -> bonds.at(0);

                    if (isAlcohol(oxygen, hydrogen))
                    {
                        // if they do make up an alcohol, return the oxygen
                        alcFound = true;
                        Alcohol returnAlc(oxygen, hydrogen);
                    }
                }
            }
        }

        // if no alcohols are found, return a nullptr
        return nullptr;
    }

    // finds all alcohol groups it can in a molecule, and returns a vector of alcohol object pointers
    std::vector<Alcohol*>* findAllAlcohol(Molecule* molecule)
    {
        std::vector<Alcohol*>* allAlcohols;
        for (int i = 0; i < molecule -> atoms.size(); i++)
        {
            // for each hydrogen in the molecule
            if (molecule -> atoms.at(i) -> element == HYDROGEN)
            {
                // check if its connected to an oxygen
                if (molecule -> atoms.at(i) -> bonds.at(0) -> element == OXYGEN)
                {
                    // check if these atoms make up an alcohol
                    Atom* hydrogen = molecule -> atoms.at(i);
                    Atom* oxygen = molecule -> atoms.at(i) -> bonds.at(0);

                    if (isAlcohol(oxygen, hydrogen))
                    {
                        // if they do make up an alcohol, return the oxygen
                        Alcohol returnAlc(oxygen, hydrogen);
                        allAlcohols -> push_back(&returnAlc);
                    }
                }
            }
        }

        // if no alcohols are found, return a nullptr
        return allAlcohols;
    }

    // finds the alcohol type (void used as it does not need a parameter)
    unsigned int alcoholType(Alcohol* alcohol)
    {
        Atom* oxygen = alcohol -> constituantAtoms.at(0);
        Atom* carbon;

        // find the attatched carbon atom
        for (int i = 0; i < oxygen -> bonds.size(); i++)
        {
            if (oxygen -> bonds.at(i) -> element == CARBON)
            {
                carbon = oxygen -> bonds.at(i);
            }
        }

        // check what type of alcohol it is (1st, 2nd, or 3rd) - determined by number of hydrogens attatched to carbon
        unsigned int hydrogenCounter = 0;
        for (int i = 0; i < carbon -> bonds.size(); i++)
        {
            if (carbon -> bonds.at(i) -> element == HYDROGEN)
            {
                hydrogenCounter++;
            }
        } 

        // if 3 hydrogens attatched, 1st degree
        // if 2 hydrogens attatched, 1st degree
        // if 1 hydrogen attatched, 2nd degree
        // if no hydrogens attatched, 3rd degree

        int alcoholClass = 3 - hydrogenCounter;
        if (alcoholClass == 0)
        {
            alcoholClass = 1;
        }

        return alcoholClass;
    }
};