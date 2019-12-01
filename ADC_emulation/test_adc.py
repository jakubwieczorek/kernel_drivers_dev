with open('/dev/my_dev0', 'r+b', buffering=0) as driver:
    driver.write(b'1000')
    print("sampling_rate =", 1000)
    for i in range(1, 10): 
        print(driver.read())

    driver.write(b'500')
    print("sampling_rate =", 500)
    for i in range(1, 10): 
        print(driver.read())
    
    driver.write(b'250')
    print("sampling_rate =", 250)
    for i in range(1, 10): 
        print(driver.read())
