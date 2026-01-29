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

// finds a carbonyl(C=O) group (in ketones, aldehydes and carbox acids, etc)
class Carbonyl: public Functional_Group
{
public:

    Carbonyl(Atom* carbon, Atom* oxygen)
    {
        constituantAtoms.at(0) = carbon;
        constituantAtoms.at(1) = oxygen;
    }

    bool isCarbonyl(Atom* atom1, Atom* atom2)
    {
        Atom* carbon;
        Atom* oxygen;

        if (atom1 -> element == CARBON && atom2 -> element == OXYGEN)
        {
            carbon = atom1;
            oxygen = atom2;
        }
        else if (atom1 -> element == OXYGEN && atom2 -> element == CARBON)
        {
            carbon = atom2;
            oxygen = atom1;
        }
        else 
        {
            return false;
        }

        // checks that the carbon has two bonds with oxygen, and if so that oxygen has two bonds with carbon
        unsigned int bondCounter = 0;
        for (int i = 0; i < carbon -> bonds.size(); i++)
        {
            if (carbon -> bonds.at(i) == oxygen)
            {
                bondCounter++;
            }
        }
        if (bondCounter = 2)
        {
            bondCounter = 0;
            for (int i = 0; i < oxygen -> bonds.size(); i++)
            {
                if (oxygen -> bonds.at(i) == carbon)
                {
                    bondCounter++;
                }
            }
            if (bondCounter == 2)
            {
                return true;
            }
        }

        // if the oxygen and carbon do not share 2 bonds, not a carbonyl
        return false;
    }

    // return the oxygen of a carbonyl
    Atom* findCarbonyl(Molecule* molecule)
    {
        bool carbonylFound = false;
        for (int i = 0; i < molecule -> atoms.size() && carbonylFound == false; i++)
        {
            if (molecule -> atoms.at(i) -> element == OXYGEN)
            {
                Atom* oxygen = molecule -> atoms.at(i);
                Atom* carbon;
                for (int j = 0; j < oxygen -> bonds.size(); j++)
                {
                    if (oxygen -> bonds.at(i) -> element == CARBON)
                    {
                        carbon = oxygen -> bonds.at(i);
                        if (isCarbonyl(carbon, oxygen))
                        {
                            return oxygen;
                        }
                    }
                }
            }
        }

        return nullptr;
    }

    Carbonyl* findCarbonyl(Molecule* molecule, bool create)
    {
        // if create is false, function returns a nullptr (and does nothing?)
        if (!create)
        {
            return nullptr;
        }

        bool carbonylFound = false;
        for (int i = 0; i < molecule -> atoms.size() && carbonylFound == false; i++)
        {
            if (molecule -> atoms.at(i) -> element == OXYGEN)
            {
                Atom* oxygen = molecule -> atoms.at(i);
                Atom* carbon;
                for (int j = 0; j < oxygen -> bonds.size(); j++)
                {
                    if (oxygen -> bonds.at(i) -> element == CARBON)
                    {
                        carbon = oxygen -> bonds.at(i);
                        if (isCarbonyl(carbon, oxygen))
                        {
                            Carbonyl returnCarbonyl(carbon, oxygen);
                            return &returnCarbonyl;
                        }
                    }
                }
            }
        }

        // else:
        return nullptr;
    }

    std::vector<Carbonyl*>* findAllCarbonyl(Molecule* molecule)
    {
        std::vector<Carbonyl*>* allCarbonyls;
        
        for (int i = 0; i < molecule -> atoms.size(); i++)
        {
            if (molecule -> atoms.at(i) -> element == OXYGEN)
            {
                Atom* oxygen = molecule -> atoms.at(i);
                Atom* carbon;
                for (int j = 0; j < oxygen -> bonds.size(); j++)
                {
                    if (oxygen -> bonds.at(i) -> element == CARBON)
                    {
                        carbon = oxygen -> bonds.at(i);
                        if (isCarbonyl(carbon, oxygen))
                        {
                            Carbonyl returnCarbonyl(carbon, oxygen);
                            allCarbonyls -> push_back(&returnCarbonyl);
                        }
                    }
                }
            }
        }

        return allCarbonyls;
    }
};