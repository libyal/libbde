# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "bdetogo.raw"

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}

@"
# libyal test data options
password=bde-TEST
"@ | Out-File -Encoding ascii -FilePath test_data_options

New-Item -Name "${TestsInputDirectory}\.libbde\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.libbde\${TestSet}\${TestFile}.password"

New-Item -Name "${TestsInputDirectory}\.pybde\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.pybde\${TestSet}\${TestFile}.password"

New-Item -Name "${TestsInputDirectory}\.bdeinfo\${TestSet}" -ItemType "directory" | Out-Null
Copy-Item test_data_options -Destination "${TestsInputDirectory}\.bdeinfo\${TestSet}\${TestFile}.password"

Remove-Item -Force -Path test_data_options
