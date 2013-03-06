static_build_raccoon{

include(./commonstatic.pro)

}else:static_build_raccoon_minisize{

include(./commonstaticminisize.pro)

}else{

include(./commondll.pro)

}