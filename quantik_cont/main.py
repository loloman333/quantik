from sklearn.tree import DecisionTreeRegressor




model = DecisionTreeRegressor()
model.fit(dataset.states, dataset.codes)

print(model.score(dataset.states, dataset.codes))