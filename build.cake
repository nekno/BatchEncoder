///////////////////////////////////////////////////////////////////////////////
// USINGS
///////////////////////////////////////////////////////////////////////////////

using System;
using System.Collections.Generic;
using System.Linq;

///////////////////////////////////////////////////////////////////////////////
// ARGUMENTS
///////////////////////////////////////////////////////////////////////////////

var target = Argument("target", "Default");

///////////////////////////////////////////////////////////////////////////////
// SETTINGS
///////////////////////////////////////////////////////////////////////////////

var platforms = new [] { "Win32", "x64" }.ToList();
var configurations = new [] { "Release" }.ToList();
var tests = new [] { "BatchEncoder.Core.UnitTests" }.ToList();
var solution = "./BatchEncoder.sln";
var versionHeaderPath = (FilePath)File("./src/version.h");
var installerScript = MakeAbsolute((FilePath)File("./setup/setup.iss"));
var iscc = @"C:/Program Files (x86)/Inno Setup 5/ISCC.exe";
var artifactsDir = (DirectoryPath)Directory("./artifacts");
var binDir = (DirectoryPath)Directory("./src/bin");
var objDir = (DirectoryPath)Directory("./src/obj");

///////////////////////////////////////////////////////////////////////////////
// RELEASE
///////////////////////////////////////////////////////////////////////////////

var isAppVeyorBuild = AppVeyor.IsRunningOnAppVeyor;
var isLocalBuild = BuildSystem.IsLocalBuild;
var isPullRequest = BuildSystem.AppVeyor.Environment.PullRequest.IsPullRequest;
var isMainRepo = StringComparer.OrdinalIgnoreCase.Equals("wieslawsoltes/BatchEncoder", BuildSystem.AppVeyor.Environment.Repository.Name);
var isMasterBranch = StringComparer.OrdinalIgnoreCase.Equals("master", BuildSystem.AppVeyor.Environment.Repository.Branch);
var isTagged = BuildSystem.AppVeyor.Environment.Repository.Tag.IsTag 
               && !string.IsNullOrWhiteSpace(BuildSystem.AppVeyor.Environment.Repository.Tag.Name);
var isRelease =  !isLocalBuild && !isPullRequest && isMainRepo && isMasterBranch && isTagged;

///////////////////////////////////////////////////////////////////////////////
// VERSION
///////////////////////////////////////////////////////////////////////////////

var text = System.IO.File.ReadAllText(versionHeaderPath.FullPath);
var split = text.Split(new char [] { '\r', '\n' }, StringSplitOptions.RemoveEmptyEntries);
var major = split[0].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var minor = split[1].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var revision = split[2].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var build = split[3].Split(new char [] { ' ' }, StringSplitOptions.RemoveEmptyEntries)[2];
var version = major + "." + minor;
var suffix = (isRelease || !isAppVeyorBuild) ? "" : "-build" + EnvironmentVariable("APPVEYOR_BUILD_NUMBER") + "-alpha";

Information("Defined Version: {0}.{1}.{2}.{3}", major, minor, revision, build);
Information("Release Version: {0}", version + suffix);

///////////////////////////////////////////////////////////////////////////////
// ACTIONS
///////////////////////////////////////////////////////////////////////////////

var buildSolutionAction = new Action<string,string,string> ((solution, configuration, platform) => 
{
    Information("Building: {0}, {1} / {2}", solution, configuration, platform);
    MSBuild(solution, settings => {
        settings.SetConfiguration(configuration);
        settings.WithProperty("Platform", "\"" + platform + "\"");
        settings.WithProperty("BuildSystem", "CI");
        settings.SetVerbosity(Verbosity.Minimal); });
});

var copyConfigAction = new Action<string> ((output) => 
{
    var outputDir = artifactsDir.Combine(output);
    var formatsDir = outputDir.Combine("formats");
    var langDir = outputDir.Combine("lang");
    var progressDir = outputDir.Combine("progress");
    var toolsDir = outputDir.Combine("tools");
    CleanDirectory(formatsDir);
    CleanDirectory(langDir);
    CleanDirectory(progressDir);
    CleanDirectory(toolsDir);
    CopyFiles("./config/formats/*.xml", formatsDir);
    CopyFiles("./config/lang/*.xml", langDir);
    CopyFiles("./config/progress/*.lua", progressDir);
    CopyFiles("./config/tools/*.xml", toolsDir);
    CopyFiles("./config/*.ps1", outputDir);
});

var packageConfigAction = new Action(() => 
{
    var output = "BatchEncoder-" + version + suffix + "-Config";
    var outputDir = artifactsDir.Combine(output);
    var outputZip = artifactsDir.CombineWithFilePath(output + ".zip");
    CleanDirectory(outputDir);
    copyConfigAction(output);
    Zip(outputDir, outputZip);
});

var packageBinariesAction = new Action<string,string> ((configuration, platform) => 
{
    var path = "./src/bin/" + configuration + "/" + platform + "/";
    var output = "BatchEncoder-" + version + suffix + "-" + platform + (configuration == "Release" ? "" : ("-(" + configuration + ")"));
    var outputDir = artifactsDir.Combine(output);
    var outputZip = artifactsDir.CombineWithFilePath(output + ".zip");
    var exeFile = File(path + "BatchEncoder.exe");
    var portableFile = File("./config/BatchEncoder.portable");
    CleanDirectory(outputDir);
    CopyFileToDirectory(File("README.md"), outputDir);
    CopyFileToDirectory(File("LICENSE.TXT"), outputDir);
    CopyFileToDirectory(exeFile, outputDir);
    CopyFileToDirectory(portableFile, outputDir);
    copyConfigAction(output);
    Zip(outputDir, outputZip);
});

var packageInstallersAction = new Action<string,string> ((configuration, platform) => 
{
    StartProcess(iscc, new ProcessSettings { 
        Arguments = 
            "\"" + installerScript.FullPath + "\""
            + " /DCONFIGURATION=" + configuration
            + " /DBUILD=" + platform
            + " /DVERSION=" + version
            + " /DSUFFIX=" + suffix, 
        WorkingDirectory = MakeAbsolute(artifactsDir) });
});

///////////////////////////////////////////////////////////////////////////////
// TASKS
///////////////////////////////////////////////////////////////////////////////

Task("Clean")
    .Does(() =>
{
    CleanDirectory(artifactsDir);
    CleanDirectory(binDir);
    CleanDirectory(objDir);
});

Task("Build")
    .IsDependentOn("Clean")
    .Does(() =>
{
    configurations.ForEach(configuration => platforms.ForEach(platform => buildSolutionAction(solution, configuration, platform)));
});

Task("Run-Unit-Tests")
    .IsDependentOn("Build")
    .Does(() =>
{
    configurations.ForEach(configuration => 
    {
        platforms.ForEach(platform => 
        {
            tests.ForEach(test => 
            {
                var pattern = "./tests/" + test + "/bin/" + configuration + "/" + platform + "/" + test + ".dll";
                VSTest(pattern, new VSTestSettings() { 
                    PlatformArchitecture = (platform == "Win32" || platform == "x86") ? VSTestPlatform.x86 : VSTestPlatform.x64,
                    InIsolation = (platform == "Win32" || platform == "x86") ? false : true,
                    Logger = "AppVeyor" });
            });
        });
    });
});

Task("Package-Binaries")
    .IsDependentOn("Run-Unit-Tests")
    .Does(() =>
{
    configurations.ForEach(configuration => platforms.ForEach(platform => packageBinariesAction(configuration, platform)));
});

Task("Package-Installers")
    .IsDependentOn("Run-Unit-Tests")
    .Does(() =>
{
    configurations.ForEach(configuration => platforms.ForEach(platform => packageInstallersAction(configuration, platform)));
});

Task("Package-Config")
    .IsDependentOn("Build")
    .Does(() =>
{
    packageConfigAction();
});

///////////////////////////////////////////////////////////////////////////////
// TARGETS
///////////////////////////////////////////////////////////////////////////////

Task("Package")
  .IsDependentOn("Package-Binaries")
  .IsDependentOn("Package-Installers")
  .IsDependentOn("Package-Config");

Task("Default")
  .IsDependentOn("Run-Unit-Tests");

///////////////////////////////////////////////////////////////////////////////
// EXECUTE
///////////////////////////////////////////////////////////////////////////////

RunTarget(target);
