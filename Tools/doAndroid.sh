cd $TOOL_HOME
echo ++++++++++++++++++++++++++++++begin CocosUi +++++++++++++++++++++++++++
echo   
./doPkmCocosUi.sh
echo ++++++++++++++++++++++++++++++begin effSpine +++++++++++++++++++++++++++
echo
./doPkmEffSpine.sh 
echo ++++++++++++++++++++++++++++++begin Spine +++++++++++++++++++++++++++
echo
./doPkmSpine.sh
echo ++++++++++++++++++++++++++++++begin Excel +++++++++++++++++++++++++++
echo
./doExcel.sh
echo ++++++++++++++++++++++++++++++begin Package +++++++++++++++++++++++++++
echo


echo ++++++++++++++++++++++++++++++ please ./doPkmPackage.sh to package the Fragments for android +++++++++++++++++++++++++++


