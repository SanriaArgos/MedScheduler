"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import Link from "next/link";
import { formatPhoneDisplay, formatPhoneForAPI, validatePhone } from '../../../utils/phoneFormatter';

export default function RegistrationPage() {
    const [formData, setFormData] = useState({
        lastName: "",
        firstName: "",
        patronymic: "",
        phone: "",
        password: "",
        confirmPassword: "",
        userType: "patient", // Default to patient
    });
    const [error, setError] = useState("");
    const [loading, setLoading] = useState(false);
    const router = useRouter();

    const handleChange = (e) => {
        const { name, value } = e.target;
        setFormData((prevData) => ({
            ...prevData,
            [name]: value,
        }));
    };

    const handlePhoneChange = (e) => {
        setFormData((prevData) => ({
            ...prevData,
            phone: e.target.value, 
        }));
    };

    const handleSubmit = async (e) => {
        e.preventDefault();
        setLoading(true);
        setError("");

        if (formData.password !== formData.confirmPassword) {
            setError("Пароли не совпадают.");
            setLoading(false);
            return;
        }

        if (!validatePhone(formData.phone)) {
            setError("Введите корректный российский номер телефона.");
            setLoading(false);
            return;
        }
        
        const formattedPhoneForAPI = formatPhoneForAPI(formData.phone);

        try {
            const response = await fetch("https://api.medscheduler.ru/register", {
                method: "POST",
                headers: { "Content-Type": "application/json" },
                body: JSON.stringify({
                    last_name: formData.lastName,
                    first_name: formData.firstName,
                    patronymic: formData.patronymic,
                    phone: formattedPhoneForAPI,
                    password: formData.password,
                    user_type: formData.userType,
                }),
            });

            const data = await response.json();

            if (response.ok && data.success) {
                // Регистрация успешна, перенаправляем на страницу входа
                router.push("/login");
            } else {
                setError(data.message || "Ошибка регистрации. Пожалуйста, попробуйте еще раз.");
            }
        } catch (err) {
            console.error("Registration error:", err);
            setError("Ошибка подключения. Пожалуйста, проверьте ваше интернет-соединение.");
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
                            <input id="lastName" name="lastName" type="text" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Фамилия" value={formData.lastName} onChange={handleChange} />
                        </div>
                        <div>
                            <label htmlFor="firstName" className="sr-only">Имя</label>
                            <input id="firstName" name="firstName" type="text" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Имя" value={formData.firstName} onChange={handleChange} />
                        </div>
                        <div>
                            <label htmlFor="patronymic" className="sr-only">Отчество (необязательно)</label>
                            <input id="patronymic" name="patronymic" type="text" className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Отчество (необязательно)" value={formData.patronymic} onChange={handleChange} />
                        </div>
                        <div>
                            <label htmlFor="phone" className="sr-only">Номер телефона</label>
                            <input
                                type="tel"
                                id="phone"
                                name="phone"
                                value={formatPhoneDisplay(formData.phone)}
                                onChange={handlePhoneChange}
                                className="w-full px-4 py-2 border border-gray-300 rounded-lg focus:ring-main2 focus:border-main2"
                                placeholder="+7 (999) 000-00-00"
                                required
                            />
                        </div>
                        <div>
                            <label htmlFor="password" className="sr-only">Пароль</label>
                            <input id="password" name="password" type="password" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Пароль (минимум 6 символов)" value={formData.password} onChange={handleChange} />
                        </div>
                        <div>
                            <label htmlFor="confirmPassword" className="sr-only">Повторите пароль</label>
                            <input id="confirmPassword" name="confirmPassword" type="password" required className="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-b-md focus:outline-none focus:ring-main focus:border-main sm:text-sm" placeholder="Повторите пароль" value={formData.confirmPassword} onChange={handleChange} />
                        </div>
                    </div>

                    {error && (
                        <p className="text-sm text-red-600 text-center py-2">{error}</p>
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

