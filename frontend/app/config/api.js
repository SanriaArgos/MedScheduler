// API Configuration
const API_BASE_URL = process.env.NODE_ENV === 'production' 
    ? 'https://api.medscheduler.ru' 
    : 'http://localhost:8080';

export default API_BASE_URL;
