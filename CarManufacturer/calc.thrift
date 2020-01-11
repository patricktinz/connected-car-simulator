struct Result {
  1: string result
}

service Calc {
  void ping(),
  /*defaultValue = "0"*/
  Result carStatus(1: string carType, 2: string numberPlate, 3: string defaultValue)
}