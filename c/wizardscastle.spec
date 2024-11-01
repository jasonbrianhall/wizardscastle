%global build_date %(date +"%Y%m%d")

Name:           wizardscastle
Version:        1.0
Release:        %{build_date}%{?dist}
Summary:        Epic text-based dungeon crawler: The Quest for the Orb of Zot

License:        MIT
URL:            https://github.com/jasonbrianhall/wizardscastle
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc, make

%description
Embark on an epic text-based adventure in Wizard's Castle, a legendary dungeon crawler reimagined for modern systems. Brave adventurer, dare you enter the Wizard's Castle in search of the fabled Orb of Zot?

Features:
• Immersive text-based gameplay with modern twists
• Randomly generated castles with 512 rooms to explore
• Encounter monsters, vendors, magical items, and more
• Choose from diverse character races: Human, Elf, Hobbit, or Dwarf
• Engage in strategic combat against fearsome foes
• Cast powerful spells and uncover ancient treasures
• Barter with the castle's enigmatic merchant
• Navigate with a retro-style ASCII map display

Every game promises a unique adventure filled with magic, monsters, and mystery. Will you emerge victorious with the Orb of Zot, or fall to the castle's many perils? Your legend begins now!

Use commands to explore, battle, and interact with the castle. Manage your resources wisely - gold, flares, and health are your lifelines. Type 'H' for help and 'M' to view the map.

Wizard's Castle: Where your choices shape your destiny!

%prep
%setup -q

%build
make
make qt


%install
mkdir -p %{buildroot}/usr/bin
install -m 0755 wizardscastle    %{buildroot}/usr/bin/wizardscastle
install -m 0755 wizardscastle-qt %{buildroot}/usr/bin/wizardscastle-qt

%files
/usr/bin/wizardscastle
/usr/bin/wizardscastle-qt

%changelog
* %(date "+%a %b %d %Y") jbhall <jasonbrianhall@gmail.com> - 1.0-%{build_date}
- Initial RPM release
