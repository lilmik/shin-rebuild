param([Parameter(Mandatory=$true)][string]$Assets,[switch]$Rebuild)
$ErrorActionPreference='Stop'
$switchRoot=$PSScriptRoot
$workspaceRoot=(Resolve-Path -LiteralPath (Join-Path $switchRoot '../..')).Path
$assetRoot=(Resolve-Path -LiteralPath $Assets).Path
$romAssets=Join-Path $switchRoot 'romfs/assets'
New-Item -ItemType Directory -Force $romAssets|Out-Null
foreach($name in @('RESOURCE.RES','_inmm.ini','RESOURCE','bgm')){Copy-Item -LiteralPath (Join-Path $assetRoot $name) -Destination $romAssets -Recurse -Force}
$forceRebuild=if($Rebuild){'1'}else{'0'}
docker run --rm -e "SHIN_FORCE_REBUILD=$forceRebuild" --mount "type=bind,source=$workspaceRoot,target=/work" -w /work/shin_rebuild/switch devkitpro/devkita64@sha256:1fc388c3a0d34bd2045a6dadcb1020e069d5f876a187fd705de14b4440c00282 bash -lc 'set -e; export PATH=/opt/devkitpro/portlibs/switch/bin:$PATH; if [ "$SHIN_FORCE_REBUILD" = 1 ]; then make -B -j4; else make -j4; fi; make -j4 CONFIG_JSON=npdm.json; mkdir -p package/exefs package/control dist; cp SHIN_MVP.nso package/exefs/main; cp SHIN_MVP.npdm package/exefs/main.npdm; cp SHIN_MVP.nacp package/control/control.nacp; cp icon.jpg package/control/icon_AmericanEnglish.dat; mv SHIN_MVP.nsp SHIN_MVP.exefs.pfs0'
if($LASTEXITCODE -ne 0){throw 'Switch compilation failed'}
python (Join-Path $switchRoot 'verify_build.py')
if($LASTEXITCODE -ne 0){throw 'Switch binary verification failed'}
