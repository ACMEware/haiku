/*
 * Copyright 2013, Stephan Aßmus <superstippi@gmx.de>.
 * Copyright 2013, Rene Gollent, <rene@gollent.com>
 *
 * All rights reserved. Distributed under the terms of the MIT License.
 */


#include "PackageAction.h"

#include "Model.h"
#include "PackageManager.h"


using namespace BPackageKit;

// #pragma mark - PackageAction


PackageAction::PackageAction(int32 type, PackageInfoRef package, Model* model)
	:
	fPackage(package),
	fType(type),
	fModel(model),
	fInstallLocation(InstallLocation(package))
{
	// TODO: ideally if the package is installed at multiple locations,
	// the user should be able to pick which one to remove.
	// TODO: allow configuring the installation location
	fPackageManager = new(std::nothrow) PackageManager(
		(BPackageInstallationLocation)fInstallLocation);
}


PackageAction::~PackageAction()
{
	delete fPackageManager;
}


PackageInfoRef
PackageAction::FindPackageByName(const BString& name)
{
	Model* model = GetModel();
	const DepotList& depots = model->Depots();
	// TODO: optimize!
	for (int32 i = 0; i < depots.CountItems(); i++) {
		const DepotInfo& depot = depots.ItemAtFast(i);
		const PackageList& packages = depot.Packages();
		for (int32 j = 0; j < packages.CountItems(); j++) {
			PackageInfoRef info = packages.ItemAtFast(j);
			if (info->Title() == name)
				return info;
		}
	}

	return PackageInfoRef();
}


int32
PackageAction::InstallLocation(const PackageInfoRef& package)
{
	const PackageInstallationLocationSet& locations
		= package->InstallationLocations();

	// If the package is already installed, return its first installed location
	if (locations.size() != 0)
		return *locations.begin();

	return B_PACKAGE_INSTALLATION_LOCATION_SYSTEM;
}


