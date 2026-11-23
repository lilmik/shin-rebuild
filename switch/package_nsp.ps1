param([Parameter(Mandatory=$true)][string]$Keyset)
$ErrorActionPreference='Stop'
$switchRoot=$PSScriptRoot
python (Join-Path $switchRoot 'prepare_control.py')
if($LASTEXITCODE -ne 0){throw 'Control/icon preparation failed'}
$keyFile=(Resolve-Path -LiteralPath $Keyset).Path
if(!(Test-Path -LiteralPath $keyFile -PathType Leaf)){throw 'Keyset file not found'}
# Do not print, copy, archive, or embed key material. Mount it read-only.
$keyText=[System.IO.File]::ReadAllText($keyFile)
if($keyText -notmatch '(?m)^\s*header_key\s*=' -or $keyText -notmatch '(?m)^\s*key_area_key_application_00\s*='){throw 'Keyset must contain header_key and key_area_key_application_00'}
$keyText=$null
docker run --rm --mount "type=bind,source=$switchRoot,target=/work" --mount "type=bind,source=$keyFile,target=/runkeys/prod.keys,readonly" -w /work devkitpro/devkita64@sha256:1fc388c3a0d34bd2045a6dadcb1020e069d5f876a187fd705de14b4440c00282 bash -lc 'make -C tools/hacBrewPack -j4 >/dev/null && ./tools/hacBrewPack/hacbrewpack --keyset /runkeys/prod.keys --titleid 0100f55100020000 --titlename "蜡笔小新1-重构版" --titlepublisher "SHIN Reconstruction" --keygeneration 1 --exefsdir package/exefs --controldir package/control --romfsdir romfs --nologo --keepncadir --nspdir dist --ncadir build/nca --tempdir build/nsp-temp --backupdir build/nsp-backup'
if($LASTEXITCODE -ne 0){throw 'NSP packaging failed'}
$built=Get-ChildItem -LiteralPath (Join-Path $switchRoot 'dist') -Filter '*.nsp'|Where-Object {$_.Name -ne 'SHIN_MVP.nsp'}|Select-Object -First 1
if($built){Copy-Item -LiteralPath $built.FullName -Destination (Join-Path $switchRoot 'dist/SHIN_MVP.nsp') -Force}
python (Join-Path $switchRoot 'verify_build.py') --nsp (Join-Path $switchRoot 'dist/SHIN_MVP.nsp')
if($LASTEXITCODE -ne 0){throw 'NSP structure verification failed'}
