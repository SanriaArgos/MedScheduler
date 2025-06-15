export const formatPhoneDisplay = (phoneInput) => {
  if (!phoneInput) return "";

  // Очищаем от всех символов кроме цифр
  let digits = phoneInput.replace(/\D/g, "");

  // Заменяем 8 на 7 в начале
  if (digits.startsWith("8")) {
    digits = "7" + digits.substring(1);
  }

  // Если ввели только +, оставляем как есть
  if (phoneInput === "+") return "+";
  
  // Если нет цифр, возвращаем пустую строку
  if (!digits) return "";

  // Если не начинается с 7, добавляем 7
  if (!digits.startsWith("7")) {
    digits = "7" + digits;
  }

  // Ограничиваем длину
  digits = digits.substring(0, 11);

  // Форматируем по частям в зависимости от длины
  let result = "+7";
  
  if (digits.length > 1) {
    result += " (";
    result += digits.substring(1, Math.min(4, digits.length));
    
    if (digits.length > 4) {
      result += ") ";
      result += digits.substring(4, Math.min(7, digits.length));
      
      if (digits.length > 7) {
        result += "-";
        result += digits.substring(7, Math.min(9, digits.length));
        
        if (digits.length > 9) {
          result += "-";
          result += digits.substring(9, 11);
        }
      }
    }
  }

  return result;
};

export const formatPhoneForAPI = (phone) => {
  if (!phone) return '';
  
  // Очищаем от всех символов кроме цифр
  const cleaned = phone.replace(/[^\d]/g, '');
  
  // Если начинается с 8, заменяем на 7
  let digits = cleaned.startsWith('8') ? '7' + cleaned.slice(1) : cleaned;
  
  // Если не начинается с 7, добавляем 7
  if (!digits.startsWith('7')) {
    digits = '7' + digits;
  }
  
  // Ограничиваем длину до 11 цифр
  digits = digits.substring(0, 11);
  
  // Возвращаем с плюсом
  return '+' + digits;
};

export const validatePhone = (phone) => {
  if (!phone) return false;
  
  const forAPI = formatPhoneForAPI(phone);
  // Проверяем формат +7 и ровно 10 цифр после
  return /^\+7\d{10}$/.test(forAPI);
};