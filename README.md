# Moisture Prediction Model

This project is designed to predict soil moisture levels based on various input features using a Random Forest Regressor model. The model is trained using a dataset with features like temperature, humidity, and crop type. It also includes functionality to predict moisture levels for new data and fetch precipitation data from an API.

## Project Structure

- `train_model.ipynb`: Jupyter notebook for training and evaluating the Random Forest Regressor model.
- `predict_moisture.py`: Script for loading the trained model and making predictions on new data.
- `fetch_data.py`: Script for fetching precipitation data from the OpenWeatherMap API and publishing data to ThingSpeak.
- `new.joblib`: Trained Random Forest Regressor model.

## Requirements

- Python 3.x
- `pandas`
- `numpy`
- `scikit-learn`
- `joblib`
- `requests`
- `urllib`
- Jupyter Notebook (for running the `.ipynb` file)

## Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/Saikat5943/smart-irrigation-project
   
