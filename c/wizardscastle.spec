Name:           wizardscastle
Version:        1.0
Release:        1%{?dist}
Summary:        Epic text-based dungeon crawler

License:        MIT
URL:            https://github.com/jasonbrianhall/wizardscastle
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc, make

%description
Embark on an epic text-based adventure in Wizard's Castle, a classic dungeon crawler reimagined for modern systems. Search for the legendary Orb of Zot!

%prep
%setup -q

%build
make

%install
mkdir -p %{buildroot}/usr/bin
install -m 0755 wizardscastle %{buildroot}/usr/bin/wizardscastle

%files
/usr/bin/wizardscastle

%changelog
* Sat Oct 12 2024 jbhall <you@example.com> - 1.0-1
- Initial RPM release
