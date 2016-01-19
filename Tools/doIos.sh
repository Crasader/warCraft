cd $TOOL_HOME
echo ++++++++++++++++++++++++++++++begin CocosUi +++++++++++++++++++++++++++
echo   
./doPvrCocosUi.sh
echo ++++++++++++++++++++++++++++++begin effSpine +++++++++++++++++++++++++++
echo
./doPvrEffSpine.sh 
echo ++++++++++++++++++++++++++++++begin Spine +++++++++++++++++++++++++++
echo
./doPvrSpine.sh
echo ++++++++++++++++++++++++++++++begin Excel +++++++++++++++++++++++++++
echo
./doExcel.sh


echo ++++++++++++++++++++++++++++++ please ./doPvrPackage.sh to package the Fragments +++++++++++++++++++++++++++



