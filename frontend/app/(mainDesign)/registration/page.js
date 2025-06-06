"use client";

import Link from "next/link";
import { useState } from "react";
import { useRouter } from 'next/navigation';

export default function RegistrationPage() {
    const [lastName, setLastName] = useState("");
    const [firstName, setFirstName] = useState("");
    const [patronymic, setPatronymic] = useState("");
    const [phone, setPhone] = useState("");
    const [password, setPassword] = useState("");
    const [repeatPassword, setRepeatPassword] = useState("");
    const [error, setError] = useState("");
    const [successMessage, setSuccessMessage] = useState("");
    const [loading, setLoading] = useState(false);
    const router = useRouter();

    const handleSubmit = async (e) => {
        e.preventDefault();
        setError("");
        setSuccessMessage("");
        setLoading(true);

        if (!lastName || !firstName || !phone || !password || !repeatPassword) {
            setError("Все поля, кроме отчества, обязательны для заполнения.");
            setLoading(false);
            return;
        }

        if (password !== repeatPassword) {
            setError("Пароли не совпадают.");
            setLoading(false);
            return;
        }

        // Валидация номера телефона (простой пример: 11 цифр)
        if (!/^\d{11}$/.test(phone)) {
            setError("Номер телефона должен состоять из 11 цифр.");
            setLoading(false);
            return;
        }

        // Валидация пароля (простой пример: минимум 6 символов)
        if (password.length < 6) {
            setError("Пароль должен содержать не менее 6 символов.");
            setLoading(false);
            return;
        }


        try {
            const registrationData = {
                last_name: lastName,
                first_name: firstName,
                phone: phone,
                password: password,
            };

            // Добавляем patronymic только если оно не пустое
            if (patronymic && patronymic.trim() !== "") {
                registrationData.patronymic = patronymic;
            }

            console.log("Отправляемые данные:", JSON.stringify(registrationData));

            const response = await fetch('https://api.medscheduler.ru/registration', {
                method: 'POST',
                headers: { 'Content-Type': 'application/json' },
                body: JSON.stringify(registrationData),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                setSuccessMessage("Регистрация прошла успешно! Теперь вы можете войти.");
                // Очистка формы или редирект через несколько секунд
                setTimeout(() => {
                    router.push('/login');
                }, 3000);
            } else {
                setError(data.error || "Ошибка регистрации. Пользователь уже существует или произошла ошибка.");
            }
        } catch (err) {
            console.error("Registration API error:", err);
            setError("Не удалось подключиться к серверу. Пожалуйста, проверьте ваше соединение.");
        } finally {
            setLoading(false);
        }
    };

    return (
        <div className="min-h-[calc(90vh-theme(space.36))] flex flex-col items-center justify-center bg-gray-50 py-12 px-4 sm:px-6 lg:px-8">
            <div className="max-w-md w-full space-y-8 bg-white p-10 rounded-xl shadow-lg">
                <div>
                    <h2 className="mt-6 text-center text-3xl font-extrabold text-main2">
                        Создание аккаунта
                    </h2>
                </div>
                <form className="mt-8 space-y-6" onSubmit={handleSubmit}>
                    <div className="rounded-md shadow-sm -space-y-px">
                        <div>
                            <label htmlFor="lastName" className="sr-only">Фамилия</label>
                            <input id="lastName" name="lastName" type="text" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Фамилия" value={lastName} onChange={(e) => setLastName(e.target.value)} />
                        </div>
                        <div>
                            <label htmlFor="firstName" className="sr-only">Имя</label>
                            <input id="firstName" name="firstName" type="text" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Имя" value={firstName} onChange={(e) => setFirstName(e.target.value)} />
                        </div>
                        <div>
                            <label htmlFor="patronymic" className="sr-only">Отчество (необязательно)</label>
                            <input id="patronymic" name="patronymic" type="text" className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Отчество (необязательно)" value={patronymic} onChange={(e) => setPatronymic(e.target.value)} />
                        </div>
                        <div>
                            <label htmlFor="phone" className="sr-only">Номер телефона</label>
                            <input id="phone" name="phone" type="tel" autoComplete="tel" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Номер телефона (11 цифр, например 79001234567)" value={phone} onChange={(e) => setPhone(e.target.value)} />
                        </div>
                        <div>
                            <label htmlFor="password" className="sr-only">Пароль</label>
                            <input id="password" name="password" type="password" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Пароль (минимум 6 символов)" value={password} onChange={(e) => setPassword(e.target.value)} />
                        </div>
                        <div>
                            <label htmlFor="repeatPassword" className="sr-only">Повторите пароль</label>
                            <input id="repeatPassword" name="repeatPassword" type="password" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-b-md focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Повторите пароль" value={repeatPassword} onChange={(e) => setRepeatPassword(e.target.value)} />
                        </div>
                    </div>

                    {error && (
                        <p className="text-sm text-red-600 text-center py-2">{error}</p>
                    )}
                    {successMessage && (
                        <p className="text-sm text-green-600 text-center py-2">{successMessage}</p>
                    )}

                    <div>
                        <button type="submit" disabled={loading} className="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-main hover:bg-main2 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-main2 transition-colors disabled:opacity-50">
                            {loading ? "Регистрация..." : "Зарегистрироваться"}
                        </button>
                    </div>
                </form>
                <div className="text-sm text-center">
                    <p className="text-gray-600">
                        Уже есть аккаунт?{' '}
                        <Link href="/login" className="font-medium text-main hover:text-main2">
                            Войти
                        </Link>
                    </p>
                </div>
            </div>
        </div>
    );
}

