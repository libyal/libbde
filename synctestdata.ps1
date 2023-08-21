# Script that synchronizes the local test data
#
# Version: 20230709

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
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$Url = "https://github.com/log2timeline/dfvfs/blob/main/test_data/${TestFile}?raw=true"

	Invoke-WebRequest -Uri ${Url} -OutFile "${TestInputDirectory}\${TestSet}\${TestFile}"

	"-pbde-TEST" | Out-File -FilePath "${TestInputDirectory}\.libbde\${TestSet}\${TestFile}.password"
	"-pbde-TEST" | Out-File -FilePath "${TestInputDirectory}\.pybde\${TestSet}\${TestFile}.password"
	"-pbde-TEST" | Out-File -FilePath "${TestInputDirectory}\.bdeinfo\${TestSet}\${TestFile}.password"
}

