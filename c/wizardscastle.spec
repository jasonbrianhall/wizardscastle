%global build_date %(date +"%Y%m%d")

Name:           wizardscastle
Version:        1.0
Release:        %{build_date}%{?dist}
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
* %(date "+%a %b %d %Y") jbhall <jasonbrianhall@gmail.com> - 1.0-%{build_date}
- Initial RPM release
