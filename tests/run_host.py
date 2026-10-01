from pathlib import Path
import argparse, subprocess, shutil, os
p=argparse.ArgumentParser();p.add_argument('--cc',default=shutil.which('gcc') or 'gcc');p.add_argument('--cjson-directory',required=True);args=p.parse_args()
root=Path(__file__).resolve().parents[1];cjson=Path(args.cjson_directory).resolve();dest=root/'.host-build';dest.mkdir(exist_ok=True)
exe=dest/('ui_tests.exe' if os.name=='nt' else 'ui_tests')
subprocess.run([args.cc,'-std=c99','-Wall','-Wextra','-Werror','-O2','-I'+str(root/'services'),'-I'+str(cjson),str(root/'services/ui_model.c'),str(root/'services/messages.c'),str(root/'tests/ui_tests.c'),str(cjson/'cJSON.c'),'-lm','-o',str(exe)],check=True)
subprocess.run([str(exe)],check=True)
