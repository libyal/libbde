# Script that synchronizes the local test data
#
# Version: 20251217

$Repository = "log2timeline/dfvfs"
$TestDataPath = "test_data"
$TestSet = "public"
$TestInputDirectory = "tests/input"
$TestFiles = "bdetogo.raw"

If (-Not (Test-Path ${TestInputDirectory}))
{
	New-Item -Name ${TestInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.libbde\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.pybde\${TestSet}" -ItemType "directory" | Out-Null
	New-Item -Name "${TestInputDirectory}\.bdeinfo\${TestSet}" -ItemType "directory" | Out-Null
}

@"
# libyal test data options
password=bde-TEST
"@ | Out-File -Encoding ascii -FilePath test_data_options

ForEach ($TestFile in ${TestFiles} -split " ")
{
	$Url = "https://raw.githubusercontent.com/${Repository}/refs/heads/main/${TestDataPath}/${TestFile}"

	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"

	Copy-Item test_data_options -Destination "${TestInputDirectory}\.libbde\${TestSet}\${TestFile}.password"
	Copy-Item test_data_options -Destination "${TestInputDirectory}\.pybde\${TestSet}\${TestFile}.password"
	Copy-Item test_data_options -Destination "${TestInputDirectory}\.bdeinfo\${TestSet}\${TestFile}.password"
}

