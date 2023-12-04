FILE=$1
sed -e "s/oneight/18/g" \
    -e "s/eightwo/82/g" \
    -e "s/twone/21/g" \
    -e "s/threeight/38/g" \
    -e "s/fiveight/58/g" \
    -e "s/eighthree/83/g" \
    -e "s/nineight/98/g" \
    -e "s/sevenine/79/g" \
    -e "s/one/1/g" \
    -e "s/two/2/g" \
    -e "s/three/3/g" \
    -e "s/four/4/g" \
    -e "s/five/5/g" \
    -e "s/six/6/g" \
    -e "s/seven/7/g" \
    -e "s/eight/8/g" \
    -e "s/nine/9/g" $FILE > $FILE.converted

bash ./1st.part1.sh $FILE.converted


