import math

dimensions = {
    "waist": 110,
    "shoulder": 120,
    "elbow": 160,
    "wrist": 90,
}

def moveTo(x, y, z):
    baseAngle = math.atan2(y, x) * 180 / math.pi

    # Length and height for the shoulder-elbow triangle
    length = math.sqrt(x**2 + y**2)
    height = z - dimensions["waist"] + dimensions["wrist"]
    
    elbowAngleRad = -math.acos((length**2 + height**2 - dimensions["shoulder"]**2 - dimensions["elbow"]**2) / (2 * dimensions["shoulder"] * dimensions["elbow"]))
    elbowAngle = elbowAngleRad * 180 / math.pi

    shoulderAngleRad = math.atan2(height, length) - math.atan2(dimensions["elbow"] * math.sin(elbowAngleRad), dimensions["shoulder"] + dimensions["elbow"] * math.cos(elbowAngleRad))
    shoulderAngle = shoulderAngleRad * 180 / math.pi

    return (baseAngle, elbowAngle, shoulderAngle)

def calculatePosition(baseAngle, elbowAngle, shoulderAngle):
    baseAngleRad = baseAngle * math.pi / 180
    elbowAngleRad = elbowAngle * math.pi / 180
    shoulderAngleRad = shoulderAngle * math.pi / 180

    length = dimensions["shoulder"] * math.cos(shoulderAngleRad) + dimensions["elbow"] * math.cos(shoulderAngleRad + elbowAngleRad)
    height = dimensions["shoulder"] * math.sin(shoulderAngleRad) + dimensions["elbow"] * math.sin(shoulderAngleRad + elbowAngleRad)

    x = length * math.cos(baseAngleRad)
    y = length * math.sin(baseAngleRad)
    z = height + dimensions["waist"] - dimensions["wrist"]

    return (x, y, z)

angles = moveTo(108, 15, 50)
print(f"Base: {angles[0]}")
print(f"Shoulder: {angles[2]}")
print(f"Elbow: {angles[1]}")

position = calculatePosition(angles[0], angles[1], angles[2])
print(f"X: {position[0]}")
print(f"Y: {position[1]}")
print(f"Z: {position[2]}")